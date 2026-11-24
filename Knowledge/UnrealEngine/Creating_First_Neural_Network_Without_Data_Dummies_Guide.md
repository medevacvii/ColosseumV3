# Starting From Zero: Create Your First Neural Network Without Data

**Beginner's hands-on guide • ColosseumV3 • PyTorch + TDD • Branch: \`albert\`**

This tutorial is a **practice exercise**. No existing dataset is necessary: we write a tiny policy to label made-up scenarios, generate examples, train an MLP, test generalization, and save a reusable model. Synthetic examples are **not equivalent to genuine game experience**; a learned model may only imitate the rules we invented.

## 1. Three approaches when you have no data

1. **Synthetic supervised learning (start here):** Write sample situations and desired actions, then generate fictional encounters with Python. Good for verifying the pipeline, but the model does not discover novel tactics.
2. **Record gameplay encounters:** Log actor ID, role, status, relationship, health, distances, chosen action and outcome. Ask team members to label good and bad decisions. Split test data by encounter and character.
3. **Reinforcement learning (later):** Train an agent in a simulated arena using carefully designed rewards, penalties, and safety constraints. Requires more infrastructure and evaluation.

**Remember:** Build the network → create examples → train → validate → save → deploy. The neural network itself does not identify other Unreal actors: Unreal's Actor references and persistent IDs provide identity, while roles/status/relationships are model features.

## 2. Our first deliberately small task

**Inputs:**

| Index | Input | Range |
|---|---|---|
| 0 | My health | 0–1 |
| 1 | Enemy currently attacking | 0 or 1 |
| 2 | Enemy surrendered | 0 or 1 |

**Outputs:** \`0=OBSERVE\`, \`1=DEFEND\`, \`2=RETREAT\`.

**Teaching rules (in priority order):**
- If surrendered, **OBSERVE** (even if an inconsistent record says attacking).
- Otherwise, if not attacking, **OBSERVE**.
- Otherwise, if health < 0.20, **RETREAT**.
- Otherwise, **DEFEND**.

These rules are teaching assumptions, not verified gameplay strategy. Enforce prohibited gameplay actions (e.g. no attack against surrendered targets) as deterministic C++ rules after ML inference.

## 3. Create the practice project in PyCharm (Windows PowerShell)

Use an isolated folder; do **not** modify shared \`.umap\` or \`.uasset\` assets.

```text
GladiatorAIPractice/
├── model.py
├── train.py
├── predict.py
├── tests/
│   └── test_model.py
└── models/                    # generated checkpoint
```

In the PyCharm PowerShell terminal, navigate to \`GladiatorAIPractice\` (paste **commands only**, without the \`PS ...>\` prompt).

```powershell
py -3.13 -m venv .venv
.\.venv\Scripts\python.exe -m pip install --upgrade pip
.\.venv\Scripts\python.exe -m pip install torch pytest
```

Use a supported installed Python version if \`py -3.13\` is unavailable. A CPU installation is sufficient for this tiny demo. Check the official [PyTorch installation selector](https://pytorch.org/get-started/locally/) for platform-specific requirements.

## 4. TDD step 1: RED — write the test before the implementation

Create \`tests/test_model.py\`:

```python
import torch
from model import GladiatorBrain


def test_network_shape():
    model = GladiatorBrain()
    inputs = torch.zeros((2, 3), dtype=torch.float32)
    scores = model(inputs)
    assert scores.shape == (2, 3)
    assert torch.isfinite(scores).all()
```

Run:

```powershell
.\.venv\Scripts\python.exe -m pytest tests/test_model.py -v
'``

**Expected RED:** \`GladiatorBrain\` does not exist yet. This is a useful failing test.

## 5. TDD step 2: GREEN — create the model

Create \`model.py\`:

```python
from torch import nn


class GladiatorBrain(nn.Module):
    def __init__(self):
        super().__init__()
        self.network = nn.Sequential(
            nn.Linear(3, 8),
            nn.ReLU(),
            nn.Linear(8, 8),
            nn.ReLU(),
            nn.Linear(8, 3),
        )

    def forward(self, x):
        return self.network(x)
```

This is a **3 → 8 → 8 → 3** multilayer perceptron. \`ReLU\` makes hidden layers nonlinear. The last layer produces three raw logits; \`CrossEntropyLoss\` expects raw logits, not Softmax probabilities.

Run the test again:

```powershell
.\.venv\Scripts\python.exe -m pytest -v
```

**Expected GREEN:** one passed. The network exists, but it is **not trained** and the test does **not** prove its decisions are good.

## 6. Generate 1,000 synthetic encounters and train

Create \`train.py\`:

```python
from pathlib import Path

import torch
from torch import nn

from model import GladiatorBrain

torch.manual_seed(120)

# Generate synthetic inputs
health = torch.rand(1000, 1)
attacking = torch.randint(0, 2, (1000, 1)).float()
surrendered = torch.randint(0, 2, (1000, 1)).float()
X = torch.cat([health, attacking, surrendered], dim=1)

# Create designer-specified labels:
# 0=OBSERVE, 1=DEFEND, 2=RETREAT
y = torch.zeros(1000, dtype=torch.long)
active_threat = (attacking[:, 0] == 1) & (surrendered[:, 0] == 0)
y[active_threat] = 1
y[active_threat & (health[:, 0] < 0.20)] = 2

print("Synthetic examples:", len(X))
print("First five inputs:\n", X[:5])
print("First five labels:\n", y[:5])

# Keep test data separate; select the model using validation only
indices = torch.randperm(1000)
train_ids = indices[:700]
val_ids = indices[700:850]
test_ids = indices[850:]

model = GladiatorBrain()
optimizer = torch.optim.Adam(model.parameters(), lr=0.01)
loss_fn = nn.CrossEntropyLoss()

best_loss = float("inf")
best_weights = None

for epoch in range(200):
    model.train()
    optimizer.zero_grad()
    logits = model(X[train_ids])
    train_loss = loss_fn(logits, y[train_ids])
    train_loss.backward()
    optimizer.step()

    model.eval()
    with torch.no_grad():
        validation_logits = model(X[val_ids])
        validation_loss = loss_fn(validation_logits, y[val_ids]).item()

    if validation_loss < best_loss:
        best_loss = validation_loss
        best_weights = {
            name: value.detach().clone()
            for name, value in model.state_dict().items()
        }

    if (epoch + 1) % 20 == 0:
        print(
            f"Epoch {epoch + 1}: "
            f"training loss={train_loss.item():.4f}, "
            f"validation loss={validation_loss:.4f}"
        )

# Restore the checkpoint selected on validation loss
model.load_state_dict(best_weights)
model.eval()

with torch.no_grad():
    predictions = model(X[test_ids]).argmax(dim=1)
    accuracy = (predictions == y[test_ids]).float().mean().item()

print(f"Held-out test accuracy: {accuracy:.1%}")

Path("models").mkdir(exist_ok=True)
torch.save(model.state_dict(), "models/gladiator_v001.pt")
print("Saved models/gladiator_v001.pt")
```

Run:

```powershell
.\.venv\Scripts\python.exe train.py
```

**Visible result:** a count of 1,000 generated examples, periodic losses, final held-out test accuracy, and \`models/gladiator_v001.pt\`.

**Do not invent the accuracy.** The actual output depends on training. This example saves the best observed validation-loss checkpoint but does **not** implement patience-based early stopping. You can add it later, test-first.

## 7. Load the saved model and predict an action

Create \`predict.py\`:

```python
import torch
from model import GladiatorBrain

model = GladiatorBrain()
model.load_state_dict(
    torch.load(
        "models/gladiator_v001.pt",
        map_location="cpu",
        weights_only=True,
    )
)
model.eval()

# Health 10%; enemy attacking; enemy not surrendered
observation = torch.tensor([[0.10, 1.0, 0.0]], dtype=torch.float32)
actions = ("OBSERVE", "DEFEND", "RETREAT")

with torch.no_grad():
    logits = model(observation)
    probabilities = torch.softmax(logits, dim=1)
    predicted_index = int(probabilities.argmax(dim=1).item())

print("Predicted action:", actions[predicted_index])
print("Action probabilities:", probabilities[0].tolist())
```

Run:

```powershell
.\.venv\Scripts\python.exe predict.py
```

**Expected rule label:** RETREAT. The **learned prediction** is whatever the actual trained model produces. If it disagrees, investigate model quality rather than claiming it passed.

## 8. Overfitting and model tests

**Overfitting** means a model learns training examples very well but performs poorly on unfamiliar situations.

| Hypothetical training accuracy | Validation accuracy | Interpretation |
|---:|---:|---|
| 55% | 54% | Probably underfitting |
| 91% | 89% | Promising |
| 99% | 72% | Possible overfitting |

The numbers are illustrations, **not** actual model results.

**TDD/quality tests to add next:**
- Fixed-seed synthetic generation is repeatable.
- All inputs are finite and in the 0–1 range; outputs are only 0, 1 or 2.
- Surrendered targets are labeled OBSERVE.
- Network output is \`[batch, 3]\`, finite.
- Training updates at least one weight on a known batch.
- Report and compare training versus validation loss/accuracy.
- If you later log multiple observations per encounter, **split by whole encounter**, not independent rows, to avoid leakage.
- If adding individual characters, evaluate on **held-out character IDs/character pairs**; never assume a model generalizes just because random rows score well.
- Evaluate surrender and low-health retreat cases separately; don't rely only on overall accuracy.

For hard gameplay prohibitions, use deterministic safeguards in C++ after inference.

## 9. Next: expand beyond the toy problem

The toy network has no explicit actor identity or relationship. In the real Unreal architecture, separate:

```text
AI Perception detects Actor
     → identity (persistent character ID)
     → role and faction
     → current status
     → relationship/memory for this target
     → normalized observation for model
     → candidate action
     → gameplay rule validator
     → Blueprint / StateTree action
```

New role/status/relationship inputs change the model schema and usually require re-training. **Do not** feed a numeric character ID as an ordered continuous input. Instead look up meaningful relationships and status from the identity/memory system.

## 10. PyTorch model → ONNX → Unreal C++

A saved \`.pt\` weights file is **not** an Unreal asset. Export a trained model with \`torch.onnx.export\`, test output parity using ONNX Runtime, then integrate an ONNX model with a compatible Unreal Neural Network Engine runtime through C++.

This guide stops at a saved PyTorch model. For exact ONNX export, TDD and Unreal steps, continue with:
- [Hands-On AI Model Creation, ONNX and TDD](AI_Model_Creation_Hands_On_TDD.md)
- [AI Model Development + C++ + Unreal + Overfitting Bible](AI_Model_Development_CPP_Unreal_TDD_Guide.md)

**Memory aid:** **DEFINE → TEST → GENERATE → TRAIN → VALIDATE → SAVE → PREDICT → EXPORT → INTEGRATE**.

## 11. Suggested Git workflow

Work only under the \`albert\` branch. Keep generated model weights and data in dedicated directories and decide whether to use Git LFS for versioned model artifacts. Do not touch production Blueprints/maps while experimenting.

```powershell
git fetch origin
git switch albert
git pull --ff-only origin albert
```

Before committing Python, run \`pytest\`, inspect \`git diff\` and \`git status\`, and stage only intended files. If this documentation is already present remotely, do not commit it again.

## References

- [PyTorch Learn the Basics](https://docs.pytorch.org/tutorials/beginner/basics/intro.html)
- [PyTorch installation selector](https://pytorch.org/get-started/locally/)
- [PyTorch ONNX exporter](https://docs.pytorch.org/docs/stable/onnx.html)
- [pytest](https://docs.pytest.org/)
- [Unreal Neural Network Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/neural-network-engine-in-unreal-engine)
