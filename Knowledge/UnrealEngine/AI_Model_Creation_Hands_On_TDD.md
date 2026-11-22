# Create Your First Gladiator AI Model — Hands-On Dummies Guide (TDD)

> **Branch:** \`albert\` · **Companion:** [AI Model Development, C++ and Unreal TDD Guide](AI_Model_Development_CPP_Unreal_TDD_Guide.md)
>
> **Result:** Build a tiny PyTorch classifier, test the Python pipeline before implementation, detect overfitting, save its weights, export an \`.onnx\` file, and compare PyTorch and ONNX predictions. Then plan the Unreal NNE/C++ interface.
>
> **Status:** Teaching example only. The code blocks below describe files to create locally; this documentation commit does **not** create Python source code, train a model, or change any Unreal assets.

## 0. Memory aid and architectural rule

\`\`\`text
WHO is it?        Unreal Actor identity / persistent character ID
WHAT are they?    Role, faction, relationship and current status
WHAT to do?       Trained model scores candidate actions
IS IT ALLOWED?    Deterministic gameplay rule / safety validator
HOW to do it?     Unreal C++ + Blueprint / StateTree / navigation / animation
\`\`\`

For a game NPC, do not train computer vision to recognize an actor already identified by Unreal. Use Actor references for runtime targets; persistent IDs for memory. **Do not feed numeric persistent IDs as continuous model features.** Read relationships from the memory store and pass meaningful features to the model.

**Initial actions:** \`OBSERVE\`, \`RESPECT\`, \`ASSIST\`, \`DEFEND\`, \`RETREAT\`. This is a toy policy—not production combat AI. Later you may add \`ATTACK\`, \`CHASE\`, \`EVADE\`, etc. Any changes to feature/action ordering require a versioned model contract and usually retraining.

## 1. Create your project in PyCharm on Windows 11

**Step 1:** In PyCharm open the ColosseumV3 repository/project. Do not create files inside \`Content/\` for this exercise.

**Step 2:** Create the directory \`MachineLearning\` at the repository root. Inside it, create folders \`gladiator_ai\`, \`tests\`, \`models\`, and \`reports\`. Inside \`gladiator_ai\` and \`tests\`, make an empty \`__init__.py\` file.

\`\`\`text
MachineLearning/
├── gladiator_ai/
│   ├── __init__.py
│   ├── features.py
│   ├── data.py
│   └── model.py
├── tests/
│   ├── __init__.py
│   ├── test_features.py
│   ├── test_data.py
│   └── test_model.py
├── models/
├── reports/
├── train.py
├── export_onnx.py
└── verify_onnx.py
\`\`\`

**Step 3:** In the PyCharm Terminal use the **actual repository root**, and enter only the commands—do not paste the \`PS C:\\...\` prompt.

\`\`\`powershell
cd MachineLearning
py -3.13 -m venv .venv
.\.venv\Scripts\python.exe -m pip install --upgrade pip
.\.venv\Scripts\python.exe -m pip install torch numpy pytest onnx onnxruntime onnxscript
.\.venv\Scripts\python.exe -m pytest --version
\`\`\`

If Python 3.13 is not installed, replace \`py -3.13\` with an installed supported Python version. Use PyCharm Settings → Project → Python Interpreter and select \`MachineLearning\\.venv\\Scripts\\python.exe\` if desired.

**Check:** You can run pytest. **Stop if installation fails**; solve that before writing the model. The first package installation can be large. For a specific GPU environment, use the official PyTorch installation selector rather than assuming CPU and CUDA packages are interchangeable.

## 2. Decide the model's contract before coding

Features are **11 numbers**, in exactly this order:

\`\`\`text
0 role_emperor          1 role_gladiator
2 role_guard            3 role_unknown
4 relationship_friendly 5 relationship_hostile
6 relationship_unknown  7 target_injured
8 target_attacking      9 target_surrendered
10 self_health_normalized
\`\`\`

Roles and relationships are one-hot encoded (one active column per category). Status flags are 0/1; health is 0–1.

Actions are **five indices**, fixed permanently for schema version 1:

\`\`\`text
0 OBSERVE   1 RESPECT   2 ASSIST   3 DEFEND   4 RETREAT
\`\`\`

An input shape is \`[batch_size, 11]\`; output is \`[batch_size, 5]\` raw **logits** (not probabilities). Interpret the highest logit index; use softmax only when you actually need probabilities. Validate inputs and version the feature schema. Unreal C++ must reproduce the **exact same feature order, normalization and action index mapping**.

## 3. RED: Write the first failing tests

Create \`tests/test_features.py\`:

\`\`\`python
import pytest
from gladiator_ai.features import ACTIONS, encode


def test_observation_has_eleven_finite_features():
    x = encode("emperor", "friendly", False, False, False, 0.9)
    assert x.shape == (11,)
    assert x.tolist()[:4] == [1.0, 0.0, 0.0, 0.0]
    assert x.tolist()[4:7] == [1.0, 0.0, 0.0]


def test_action_contract_is_stable():
    assert ACTIONS == ("OBSERVE", "RESPECT", "ASSIST", "DEFEND", "RETREAT")


def test_invalid_health_is_rejected():
    with pytest.raises(ValueError):
        encode("gladiator", "hostile", False, True, False, 1.5)
\`\`\`

Run:

\`\`\`powershell
.\.venv\Scripts\python.exe -m pytest tests/test_features.py -v
\`\`\`

**Expected RED:** import fails because \`gladiator_ai.features\` has not been implemented. This is intentional.

## 4. GREEN: Implement feature encoding

Create \`gladiator_ai/features.py\`:

\`\`\`python
import math
import numpy as np

ROLES = ("emperor", "gladiator", "guard", "unknown")
RELATIONSHIPS = ("friendly", "hostile", "unknown")
ACTIONS = ("OBSERVE", "RESPECT", "ASSIST", "DEFEND", "RETREAT")


def encode(role, relationship, injured, attacking, surrendered, self_health):
    if role not in ROLES or relationship not in RELATIONSHIPS:
        raise ValueError("Unknown role or relationship")
    if not math.isfinite(float(self_health)) or not 0 <= self_health <= 1:
        raise ValueError("self_health must be in [0, 1]")
    if not all(type(flag) is bool for flag in (injured, attacking, surrendered)):
        raise ValueError("Status flags must be booleans")
    values = (
        [float(role == r) for r in ROLES]
        + [float(relationship == r) for r in RELATIONSHIPS]
        + [float(injured), float(attacking), float(surrendered), float(self_health)]
    )
    return np.asarray(values, dtype=np.float32)
\`\`\`

Run the exact same pytest command. **Expected GREEN:** 3 passed.

**REFACTOR:** Only after passing, add docstrings and improve organization. Rerun tests after each change.

## 5. Write a reference policy to label synthetic data

For teaching purposes we need known expected answers. These are **designer rules**, not learned predictions:

1. Surrendered target → OBSERVE.
2. Low self-health (\`< 0.2\`) while facing attacking hostile target → RETREAT.
3. Friendly emperor → RESPECT.
4. Injured friendly target → ASSIST.
5. Attacking hostile target → DEFEND.
6. Otherwise → OBSERVE.

Do not confuse these synthetic labels with authentic gameplay performance. For a real model, review the decision policy with your team and collect human-approved or simulated encounter outcomes.

Create \`tests/test_data.py\` **first**:

\`\`\`python
from gladiator_ai.data import expected_action, generate_encounters, split_encounters


def test_reference_surrender():
    assert expected_action("gladiator", "hostile", False, False, True, 0.8) == 0


def test_reference_caesar():
    assert expected_action("emperor", "friendly", False, False, False, 0.8) == 1


def test_reference_injured_ally():
    assert expected_action("gladiator", "friendly", True, False, False, 0.8) == 2


def test_reference_hostile_defense():
    assert expected_action("gladiator", "hostile", False, True, False, 0.8) == 3


def test_reference_low_health_retreat():
    assert expected_action("gladiator", "hostile", False, True, False, 0.1) == 4


def test_encounters_never_leak_between_splits():
    records = generate_encounters(120, seed=120)
    train, valid, test = split_encounters(records, seed=120)
    sets = [{r["encounter_id"] for r in part} for part in (train, valid, test)]
    assert sets[0].isdisjoint(sets[1])
    assert sets[0].isdisjoint(sets[2])
    assert sets[1].isdisjoint(sets[2])
    assert all(sets)
\`\`\`

Run pytest now (RED), then create \`gladiator_ai/data.py\`:

\`\`\`python
import random
from .features import ACTIONS, ROLES, RELATIONSHIPS, encode


def expected_action(role, relationship, injured, attacking, surrendered, self_health):
    if surrendered:
        return ACTIONS.index("OBSERVE")
    if relationship == "hostile" and attacking and self_health < 0.20:
        return ACTIONS.index("RETREAT")
    if role == "emperor" and relationship == "friendly":
        return ACTIONS.index("RESPECT")
    if relationship == "friendly" and injured:
        return ACTIONS.index("ASSIST")
    if relationship == "hostile" and attacking:
        return ACTIONS.index("DEFEND")
    return ACTIONS.index("OBSERVE")


def generate_encounters(n=1200, seed=120):
    rng = random.Random(seed)
    records = []
    for encounter in range(n):
        role = rng.choice(ROLES)
        relation = rng.choice(RELATIONSHIPS)
        injured = bool(rng.getrandbits(1))
        attacking = bool(rng.getrandbits(1))
        surrendered = bool(rng.getrandbits(1))
        health = rng.random()
        label = expected_action(role, relation, injured, attacking, surrendered, health)
        # Two observations per encounter: never place them in different partitions.
        for observation in range(2):
            records.append({
                "encounter_id": encounter,
                "observation_id": observation,
                "target_id": f"npc_{rng.randrange(40):03d}",
                "features": encode(role, relation, injured, attacking, surrendered, health),
                "label": label,
            })
    return records


def split_encounters(records, seed=120):
    groups = sorted({r["encounter_id"] for r in records})
    if len(groups) < 3:
        raise ValueError("At least three distinct encounters required")
    random.Random(seed).shuffle(groups)
    n = len(groups)
    n_train = int(n * 0.70)
    n_valid = int(n * 0.15)
    train_ids = set(groups[:n_train])
    valid_ids = set(groups[n_train:n_train + n_valid])
    test_ids = set(groups[n_train + n_valid:])
    return (
        [r for r in records if r["encounter_id"] in train_ids],
        [r for r in records if r["encounter_id"] in valid_ids],
        [r for r in records if r["encounter_id"] in test_ids],
    )
\`\`\`

Run:

\`\`\`powershell
.\.venv\Scripts\python.exe -m pytest -v
\`\`\`

**Important:** These synthetic encounters intentionally include simplified, sometimes unrealistic combinations of status. In the actual simulation, constrain impossible states (such as simultaneous attacking and surrendering) and test those constraints. The \`target_id\` is present only as encounter metadata; **it is not a neural-network feature**.

For stronger generalization claims, add a separate **held-out target ID**, target-pair and time-based evaluation. This first example guarantees only encounter-group separation.

## 6. RED → GREEN: Implement the PyTorch model

Create \`tests/test_model.py\`:

\`\`\`python
import torch
from gladiator_ai.model import GladiatorBrain


def test_model_shape_and_finite_values():
    model = GladiatorBrain()
    logits = model(torch.zeros((2, 11), dtype=torch.float32))
    assert logits.shape == (2, 5)
    assert torch.isfinite(logits).all()
\`\`\`

Run pytest (RED), then create \`gladiator_ai/model.py\`:

\`\`\`python
import torch.nn as nn


class GladiatorBrain(nn.Module):
    def __init__(self):
        super().__init__()
        self.network = nn.Sequential(
            nn.Linear(11, 32),
            nn.ReLU(),
            nn.Linear(32, 32),
            nn.ReLU(),
            nn.Linear(32, 5),
        )

    def forward(self, observations):
        return self.network(observations)
\`\`\`

Run pytest again (GREEN). This only proves model shape, **not that the model is smart**.

## 7. Train the AI, while watching for overfitting

Create \`train.py\`:

\`\`\`python
import copy
import json
import random
from pathlib import Path

import numpy as np
import torch
from torch import nn

from gladiator_ai.data import generate_encounters, split_encounters
from gladiator_ai.features import ACTIONS
from gladiator_ai.model import GladiatorBrain

torch.manual_seed(120)
np.random.seed(120)
random.seed(120)

Path("models").mkdir(exist_ok=True)
Path("reports").mkdir(exist_ok=True)

records = generate_encounters(1200, seed=120)
train, valid, test = split_encounters(records, seed=120)


def arrays(part):
    x = torch.tensor(np.stack([r["features"] for r in part]))
    y = torch.tensor([r["label"] for r in part], dtype=torch.long)
    return x, y


x_train, y_train = arrays(train)
x_valid, y_valid = arrays(valid)
x_test, y_test = arrays(test)

model = GladiatorBrain()
optimizer = torch.optim.Adam(model.parameters(), lr=0.003, weight_decay=1e-4)
criterion = nn.CrossEntropyLoss()

best_loss = float("inf")
best_weights = None
best_epoch = 0
patience = 20
stale_epochs = 0
history = []

for epoch in range(1, 201):
    model.train()
    optimizer.zero_grad()
    train_logits = model(x_train)
    loss = criterion(train_logits, y_train)
    loss.backward()
    optimizer.step()

    model.eval()
    with torch.no_grad():
        valid_logits = model(x_valid)
        valid_loss = criterion(valid_logits, y_valid).item()
        train_accuracy = (model(x_train).argmax(1) == y_train).float().mean().item()
        valid_accuracy = (valid_logits.argmax(1) == y_valid).float().mean().item()

    history.append({
        "epoch": epoch,
        "train_loss": float(loss.item()),
        "valid_loss": valid_loss,
        "train_accuracy": train_accuracy,
        "valid_accuracy": valid_accuracy,
    })

    if valid_loss < best_loss - 1e-4:
        best_loss = valid_loss
        best_weights = copy.deepcopy(model.state_dict())
        best_epoch = epoch
        stale_epochs = 0
    else:
        stale_epochs += 1
    if epoch == 1 or epoch % 20 == 0:
        print(f"Epoch {epoch:3} | train={train_accuracy:.1%} | valid={valid_accuracy:.1%}")
    if stale_epochs >= patience:
        print("Early stopping: validation loss stopped improving")
        break

model.load_state_dict(best_weights)
model.eval()
with torch.no_grad():
    train_predictions = model(x_train).argmax(1)
    valid_predictions = model(x_valid).argmax(1)
    test_predictions = model(x_test).argmax(1)
    train_acc = (train_predictions == y_train).float().mean().item()
    valid_acc = (valid_predictions == y_valid).float().mean().item()
    test_acc = (test_predictions == y_test).float().mean().item()

def recall_for_class(predictions, labels, class_index):
    actual = labels == class_index
    if not actual.any().item():
        return None
    return float((predictions[actual] == class_index).float().mean().item())

retreat_idx = ACTIONS.index("RETREAT")
report = {
    "schema_version": 1,
    "seed": 120,
    "best_epoch": best_epoch,
    "train_accuracy": train_acc,
    "validation_accuracy": valid_acc,
    "test_accuracy": test_acc,
    "validation_retreat_recall": recall_for_class(valid_predictions, y_valid, retreat_idx),
    "test_retreat_recall": recall_for_class(test_predictions, y_test, retreat_idx),
    "train_encounters": len({r["encounter_id"] for r in train}),
    "validation_encounters": len({r["encounter_id"] for r in valid}),
    "test_encounters": len({r["encounter_id"] for r in test}),
}
torch.save(model.state_dict(), "models/GladiatorBrain_v001.pt")
Path("reports/training_history.json").write_text(json.dumps(history, indent=2))
Path("reports/evaluation.json").write_text(json.dumps(report, indent=2))
print(json.dumps(report, indent=2))
\`\`\`

Run:

\`\`\`powershell
.\.venv\Scripts\python.exe train.py
\`\`\`

**Visible result:** Terminal prints training and validation accuracy every 20 epochs and a final report. You will also see:
- \`models/GladiatorBrain_v001.pt\`: trained PyTorch weights.
- \`reports/training_history.json\`: one record per epoch, for plotting learning curves.
- \`reports/evaluation.json\`: actual measured metrics from the run.

The **test set must remain untouched during model selection**. This script uses validation loss to select the best weights and computes test results only after training ends. The accuracy numbers vary with software/hardware; do not fabricate or expect an exact number.

## 8. Overfitting: check before allowing deployment

Read \`reports/evaluation.json\`. The key question is: **Does high training performance transfer to separate validation and test encounters?**

Simple warning signal:

\`\`\`text
Train accuracy  99%
Valid accuracy  72%
               ↑
Large gap = possible overfitting
\`\`\`

If both training and validation are poor, suspect **underfitting** instead. Neither a small gap nor a high single score guarantees a useful model.

Write a **post-training acceptance test** that reads the saved report; it must not retrain a random model on each pytest run:

\`\`\`python
# tests/test_evaluation_report.py
import json
from pathlib import Path

import pytest


def test_model_generalization_gate():
    path = Path("reports/evaluation.json")
    if not path.exists():
        pytest.skip("Train the model first: python train.py")
    report = json.loads(path.read_text())
    assert report["validation_accuracy"] >= 0.90
    assert report["train_accuracy"] - report["validation_accuracy"] <= 0.10
\`\`\`

**The 90% and 10-percentage-point thresholds are example targets.** Agree on project requirements *before* evaluating. Also check per-class recall, confusion matrix, class counts, and cases involving surrender, unknown targets and rare roles. A rare class can fail while accuracy appears high.

Good countermeasures include **early stopping**, a smaller model, weight decay, dropout, better representative encounter data, and ensuring proper group splits. Do not simply keep training longer when validation quality is getting worse.

**A deliberate tiny-batch overfit test** can be useful in development to check that gradients and labels work. It is a debugging test—not evidence of generalization.

## 9. Export the trained brain to ONNX

Create \`export_onnx.py\`:

\`\`\`python
from pathlib import Path
import torch

from gladiator_ai.model import GladiatorBrain

model = GladiatorBrain()
model.load_state_dict(
    torch.load("models/GladiatorBrain_v001.pt", map_location="cpu", weights_only=True)
)
model.eval()
dummy_input = torch.zeros((1, 11), dtype=torch.float32)
Path("models").mkdir(exist_ok=True)
torch.onnx.export(
    model,
    (dummy_input,),
    "models/GladiatorBrain_v001.onnx",
    input_names=["observation"],
    output_names=["decision_logits"],
    dynamo=True,
)
print("Exported models/GladiatorBrain_v001.onnx")
\`\`\`

Run:

\`\`\`powershell
.\.venv\Scripts\python.exe export_onnx.py
\`\`\`

**Expected:** The ONNX file is created. Some exporter versions may also produce sidecar files or console messages; inspect any errors instead of assuming export worked.

## 10. Verify PyTorch vs ONNX

Create \`verify_onnx.py\`:

\`\`\`python
import numpy as np
import onnxruntime as ort
import torch

from gladiator_ai.features import encode, ACTIONS
from gladiator_ai.model import GladiatorBrain

model = GladiatorBrain()
model.load_state_dict(
    torch.load("models/GladiatorBrain_v001.pt", map_location="cpu", weights_only=True)
)
model.eval()

example = encode(
    role="gladiator",
    relationship="hostile",
    injured=False,
    attacking=True,
    surrendered=False,
    self_health=0.10,
).reshape(1, 11)

with torch.no_grad():
    pytorch_logits = model(torch.from_numpy(example)).numpy()

session = ort.InferenceSession(
    "models/GladiatorBrain_v001.onnx",
    providers=["CPUExecutionProvider"],
)
onnx_logits = session.run(
    ["decision_logits"],
    {"observation": example},
)[0]

np.testing.assert_allclose(pytorch_logits, onnx_logits, rtol=1e-4, atol=1e-5)
decision = ACTIONS[int(onnx_logits.argmax(axis=1)[0])]
print("PASS: PyTorch and ONNX outputs match within tolerance")
print("Selected action:", decision)
\`\`\`

Run:

\`\`\`powershell
.\.venv\Scripts\python.exe verify_onnx.py
\`\`\`

**Expected:** Numerical parity passes and prints a decision. A trained model may still make a poor decision! The predicted action here is an **observation**, not proof that model-quality criteria were met. Run separate tests for multiple examples and model-quality gates.

## 11. Add hard gameplay constraints

For the game, don't make a surrender prohibition depend only on the neural model. Keep an authoritative validator **after** inference:

\`\`\`text
NNE predicts ATTACK
          |
          v
Gameplay policy checks:
target surrendered?
          |
          +--- YES ---> block attack; select safe fallback
          |
          +--- NO ----> allowed action proceeds
\`\`\`

Write an Unreal C++ Automation Test **first**: \`Surrendered target -> Attack rejected\`. The exact combat implementation is project-specific. Keep it in C++ and expose clean decision/action interfaces to Blueprint.

## 12. Integrate with C++ and Unreal Engine 5.8

**Do not drop ONNX directly onto a Gladiator Blueprint and expect it to work.** Follow the existing [architecture and TDD guide](AI_Model_Development_CPP_Unreal_TDD_Guide.md):

1. Enable an appropriate NNE runtime plugin in the Unreal editor; verify the runtime is supported for the project's platform and engine build.
2. Add the engine's required module dependencies to the relevant \`.Build.cs\` file, per the official NNE documentation for the **installed engine version**.
3. Import the ONNX file so Unreal can create a neural model asset (\`UNNEModelData\`, as supported by the runtime).
4. Write a C++ test for model loading, input \`[1,11]\`, output \`[1,5]\` and consistent action-index decoding.
5. Implement the NNE adapter behind \`IAIDecisionProvider\`; preserve a rule-based fallback for unavailable runtimes or bad inputs.
6. Create a Blueprint-callable facade to return an \`EInteractionAction\` and explicit target Actor. Blueprint or StateTree executes that decision.
7. Add an isolated functional test map. Check respect for Caesar, assisting an injured ally, hostile combat, surrender, unknown identity and model-unavailable fallback.
8. Re-run Python, parity, C++ and Blueprint regression tests before code review.

**Crucial:** This beginner exercise is **not yet an Unreal-integrated production feature**. The ONNX/Unreal portion needs project-specific C++ implementation and verification. Do not modify the shared \`.uasset\` or \`.umap\` files without coordinating with asset owners.

## 13. Version and preserve the trained brain

\`\`\`text
MachineLearning/
├── models/
│   ├── GladiatorBrain_v001.pt
│   └── GladiatorBrain_v001.onnx
└── reports/
    ├── training_history.json
    └── evaluation.json
\`\`\`

Also record a machine-readable schema description, dataset-generation/training version, relevant dependency versions, chosen evaluation thresholds and model hash for traceability. Decide as a team whether binary models should use Git LFS or external artifacts; avoid accidentally committing massive checkpoints to ordinary Git. \`.venv\`, generated caches and temporary results generally belong in \`.gitignore\`.

**No retraining on Unreal startup.** Train offline, export and version the model, load it in Unreal, and provide current observations/relationships/memory during inference.

## 14. Pre-merge TDD checklist

- [ ] Test definitions and acceptance criteria written before implementation.
- [ ] Witnessed RED then GREEN for features, synthetic data and model contract.
- [ ] Input schema/action order versioned and independently testable.
- [ ] Encounter groups never cross train/validation/test boundaries.
- [ ] Trained model evaluated on held-out encounters; additional unknown-person tests planned.
- [ ] Overfitting gap, per-class outcomes and learning curves reviewed.
- [ ] Best checkpoint restored by early stopping.
- [ ] PyTorch and ONNX predictions agree within numerical tolerances.
- [ ] Runtime errors have explicit C++ fallback.
- [ ] Surrender/safety restrictions enforced outside stochastic model.
- [ ] Unreal C++ Automation and Blueprint Functional Tests pass.
- [ ] No production Blueprint/map files touched accidentally.

## 15. Next hands-on exercise

Once all Python stages work, train a second model **only after adding a failing test** for a new requirement (e.g. target surrender after attacking). Compare \`v001\` and \`v002\` on the **same** unchanged evaluation suite; save both so you can roll back.

**Memory aid: TEST → ENCODE → TRAIN → VALIDATE → CHECK OVERFITTING → EXPORT → COMPARE → UNREAL.**

### Official references

- [PyTorch: Installation](https://pytorch.org/get-started/locally/)
- [PyTorch: ONNX export](https://docs.pytorch.org/docs/stable/onnx.html)
- [pytest documentation](https://docs.pytest.org/)
- [ONNX Runtime Python guide](https://onnxruntime.ai/docs/get-started/with-python.html)
- [Unreal Engine Neural Network Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/neural-network-engine-in-unreal-engine)
