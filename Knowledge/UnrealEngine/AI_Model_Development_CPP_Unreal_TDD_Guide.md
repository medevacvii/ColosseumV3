# AI Model Development and Integration with C++ and Unreal Engine — Beginner's Guide

> **Branch:** `albert` · **Last updated:** 2026-11-21 · **Status:** Architecture and test-first development guide (not an implementation claim)
>
> **Goal:** Develop reusable trained AI models for Colosseum characters, run inference through C++ in Unreal Engine, and let Blueprints execute actions. **Test-driven development (TDD) is required throughout.**

## 1. The big picture

```text
Unreal AI Perception / character stats / world state
                      |
                      v
               Context Collector  <---- Short/long-term memory
                      |
                      v
              Versioned observation
                      |
                      v
            IAIDecisionProvider
                /           \
      Rule-based fallback   Neural provider
                                  |
                                  v
                         ONNX via Unreal NNE
                \           /
                 v         v
                EGladiatorDecision
                         |
                         v
           StateTree / Blueprint behaviour
                         |
                         v
             Navigate / attack / retreat
```

**Remember:** AI chooses **what** to do; Unreal determines **how** to do it. A saved model does **not** need retraining when Unreal restarts. Context awareness means passing fresh observations and relevant memory into inference, not training continuously.

## 2. Vocabulary

| Term | Beginner's meaning |
|---|---|
| Training | Change model weights using examples or rewards |
| Model | Saved learned parameters and network structure |
| Inference | Get a prediction from a trained model |
| ONNX | Portable model format (`.onnx`, not `.nnx`) |
| NNE | Unreal's Neural Network Engine inference interface |
| Observation | Snapshot of information about the character and world |
| Context memory | State from past events, such as last-seen enemy position |
| TDD | Write failing test → minimally implement → refactor |
| Regression | A behaviour that used to work stops working |

## 3. Recommended architecture (high cohesion, low coupling)

Keep these responsibilities separate:

1. `AIContextComponent`: reads AI Perception, navigation, health, enemies, allies, world conditions.
2. `AIMemoryComponent`: stores last-seen enemy position, time since last sighting, recent damage and similar remembered context.
3. `FAIObservation`: versioned data transfer contract; numerical values normalized identically in Python and C++.
4. `IAIDecisionProvider`: Strategy interface used by a rules provider and a neural provider.
5. `NeuralDecisionProvider`: owns NNE model loading, input/output tensors, inference, and errors; never controls animations directly.
6. `AIModelComponent`: high-level Blueprint-facing orchestration, with fallback when neural inference is unavailable.
7. Blueprint or StateTree: executes `Attack`, `Chase`, `Defend`, `Retreat`, etc., using existing Unreal gameplay systems.

**Design patterns:** Strategy for replaceable decision engines; Adapter for NNE/ONNX binding; Facade for Blueprint-facing API; Dependency Inversion to avoid concrete model dependencies; optionally Observer/event dispatch for context updates. No single Blueprint should be responsible for tensors, memory, training, movement and combat.

## 4. Build a tiny first prototype

**Question:** Attack, chase, defend, or retreat?

Start with six input features:

| Index | Field | Example | Encoding |
|---|---|---:|---|
| 0 | `health` | 0.80 | 0–1 |
| 1 | `enemy_health` | 0.35 | 0–1 |
| 2 | `enemy_distance` | 0.20 | divide cm by an agreed maximum and clamp 0–1 |
| 3 | `can_see_enemy` | 1 | 0/1 |
| 4 | `enemy_attacking` | 1 | 0/1 |
| 5 | `nearby_allies` | 0.25 | agreed normalization |

Outputs **must have one unchanging order**:

| Index | Output |
|---|---|
| 0 | `ATTACK` |
| 1 | `CHASE` |
| 2 | `DEFEND` |
| 3 | `RETREAT` |

Store the input/output ordering, scaling constants, tensor shapes, dtypes and a schema version alongside the model; changing the order silently produces wrong decisions.

## 5. Mandatory TDD: RED → GREEN → REFACTOR

**RED:** Write a meaningful failing test and run it. **GREEN:** Implement the smallest behaviour to pass. **REFACTOR:** Improve code without changing behaviour and rerun tests.

For learned models, distinguish deterministic **software-contract tests** (exact expected values are appropriate) from **model-quality evaluation** (use held-out data, metrics and tolerances rather than claiming a stochastic training run always produces one exact decision).

### Exercise 1 — First test before implementation

File: `MachineLearning/tests/test_decisions.py`

```python
from gladiator_ai.decisions import decide

def test_low_health_retreat():
    assert decide(
        health=0.10,
        enemy_health=0.90,
        distance=0.20
    ) == "RETREAT"
```

Run in the `MachineLearning` directory:

```powershell
python -m pytest tests/test_decisions.py -v
```

Expected RED: the import/function does not exist yet.

Now create `MachineLearning/gladiator_ai/decisions.py`:

```python
def decide(health: float, enemy_health: float, distance: float) -> str:
    if health < 0.20:
        return "RETREAT"
    return "ATTACK"
```

Run the test again: GREEN. This deliberately limited rule is a **baseline/fallback**, not a trained model.

Next tests to write **before implementation**: no enemy → no invalid attack, far visible enemy → chase, imminent attack → defend, invalid input → safe fallback; keep the decisions consistent with the project's agreed behaviour specification.

### Exercise 2 — Test a neural model's contract

File: `MachineLearning/tests/test_model.py`

```python
import torch
from gladiator_ai.model import GladiatorBrain

def test_single_observation_yields_four_finite_logits():
    model = GladiatorBrain()
    observation = torch.zeros((1, 6), dtype=torch.float32)
    output = model(observation)
    assert output.shape == (1, 4)
    assert torch.isfinite(output).all()
```

Then implement the minimal architecture:

```python
import torch
from torch import nn

class GladiatorBrain(nn.Module):
    def __init__(self):
        super().__init__()
        self.network = nn.Sequential(
            nn.Linear(6, 16), nn.ReLU(),
            nn.Linear(16, 16), nn.ReLU(),
            nn.Linear(16, 4),
        )

    def forward(self, x):
        return self.network(x)
```

This proves output shape and finiteness, **not** model intelligence.

### Exercise 3 — Training TDD and model validation

Before implementing `train.py`, test that:
- Input and label dimensions are correct and values are finite.
- The feature schema and normalization are consistent.
- An optimizer step updates at least one trainable parameter on valid data.
- A small fixed synthetic dataset can be overfit (sanity check).
- Training/validation/test splits prevent leakage between correlated samples (for example, keep related fights in the same split).
- Seeds, data version, hyperparameters, metrics and software versions are recorded.
- Trained weights can be saved, loaded and used for inference.

On a held-out dataset evaluate accuracy, confusion matrix and **per-action precision/recall** (especially safety-sensitive choices such as retreat). Define acceptable metrics in the issue/PR **before training**, then compare results. Re-run evaluation after retraining. Random seeds improve reproducibility but do not guarantee identical outputs on every backend.

### Exercise 4 — Export and ONNX parity test

Export a model in evaluation mode with example input `float32[1,6]`:

```python
model.eval()
example = torch.zeros((1, 6), dtype=torch.float32)
torch.onnx.export(
    model, (example,), "GladiatorBrain_v001.onnx",
    input_names=["observation"],
    output_names=["decision_logits"],
    dynamo=True
)
```

**Before connecting Unreal**, write tests that use ONNX Runtime to verify:
- ONNX loads, has input `observation` and output `decision_logits`.
- Input/output dimensions and dtypes match the versioned contract.
- PyTorch and ONNX outputs are numerically close on a representative batch (e.g. use `numpy.testing.assert_allclose` with explicitly chosen tolerances).
- The predicted action index agrees on representative examples without near-tied logits.
- A missing or mismatched file is handled as a controlled error.

Export the model after training and store a retrainable PyTorch checkpoint as well. `.pt` is a development checkpoint; `.onnx` is the deployment artifact.

### Exercise 5 — C++ tests before integration

Write Unreal Automation Tests for:
- Normalization and feature ordering.
- Valid enum mapping of model indices.
- Model missing/runtime unavailable → rule-based fallback.
- Invalid observation/tensor shape → controlled failure, no crash.
- An imported `UNNEModelData` asset and chosen NNE runtime can load and infer with the intended tensor contract.
- A repeatable test observation yields a valid action.

Example **design sketch**, assuming the proposed header/implementation exist:

```cpp
#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "AI/Rules/RuleDecisionProvider.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGladiatorLowHealthTest,
    "Colosseum.AI.Rules.LowHealthRetreat",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FGladiatorLowHealthTest::RunTest(const FString& Parameters)
{
    FGladiatorAIObservation Input;
    Input.Health = 0.10f;
    Input.EnemyHealth = 0.90f;
    Input.EnemyDistance = 0.20f;
    TestEqual(
        TEXT("Low health means retreat"),
        FRuleDecisionProvider::Decide(Input),
        EGladiatorDecision::Retreat
    );
    return true;
}
#endif
```

This sample is **not drop-in compilable** until the `FRuleDecisionProvider`, observation and enum types exist. Verify actual NNE API names and selected runtime against the installed engine version before writing production integration code.

### Exercise 6 — Blueprint functional testing

Use an isolated test map/actor (not a shared production `.umap`). Test scenarios:

1. High health + weak nearby enemy → expected aggressive decision.
2. Low health + strong nearby enemy → retreat decision and visible retreat behaviour.
3. Enemy lost from sight → investigate *last seen position* using context memory (once that action exists).
4. Missing ONNX or unavailable runtime → fallback still operates.
5. Reload level / restart Unreal → existing deployed model runs without retraining.
6. Memory reset rules are explicit: temporary state resets appropriately; persistent state is saved only when intended.

Use Unreal Automation/Functional Tests, and log the input schema version, model version, chosen provider and resulting decision to aid reproducibility.

## 6. Proposed project layout

```text
ColosseumV3/
├── Knowledge/UnrealEngine/
│   └── AI_Model_Development_CPP_Unreal_TDD_Guide.md
├── MachineLearning/                  # future implementation
│   ├── gladiator_ai/
│   │   ├── decisions.py
│   │   ├── model.py
│   │   ├── training.py
│   │   └── export_onnx.py
│   ├── tests/
│   │   ├── test_decisions.py
│   │   ├── test_model.py
│   │   ├── test_training.py
│   │   ├── test_evaluation.py
│   │   └── test_onnx.py
│   └── models/
└── Source/ColosseumV3/               # integrate with existing project conventions
    └── ... AI interfaces, providers, components and Automation Tests
```

The tree above is a **target design**, not a claim that those files currently exist. In particular, inspect existing project classes and module names before introducing new C++ folders.

## 7. Test gates for pull requests

- [ ] Feature requirement and observable acceptance criteria defined.
- [ ] Failing automated test committed or demonstrated before implementation.
- [ ] Python unit/data/contract tests pass.
- [ ] Training results use fixed, uncontaminated evaluation data with recorded metrics.
- [ ] Trained checkpoint and ONNX versions are traceable to data, schema and code.
- [ ] ONNX loads and PyTorch/ONNX parity passes.
- [ ] Unreal C++ automation tests pass.
- [ ] Blueprint functional/regression tests pass on isolated test assets.
- [ ] Failure/fallback behaviour tested.
- [ ] No shared `.uasset` or `.umap` files changed without asset owner coordination.
- [ ] Documentation updated; peer review before merge to `main`.

## 8. Suggested implementation sprints

1. **S01 — Requirements, schema and rule-based baseline with pytest.**
2. **S02 — PyTorch architecture and training tests; evaluation metrics.**
3. **S03 — ONNX export, contract and numerical parity.**
4. **S04 — C++ NNE adapter and fallback; automation tests.**
5. **S05 — Blueprint/StateTree action wiring; functional tests.**
6. **S06 — Context memory and persistence; regression tests.**

Do not combine all six into a single large untestable change.

## 9. Team-safe Git workflow

Work on `albert`. Keep AI documentation and prototype code in separate commits when possible; do not overwrite collaborators' Blueprints/maps.

```powershell
git fetch origin
git switch albert
git pull --ff-only origin albert
# Review and test the changes before committing.
git add Knowledge/UnrealEngine/AI_Model_Development_CPP_Unreal_TDD_Guide.md
git commit -m "docs(ai): add test-driven Python ONNX C++ Unreal guide"
git push origin albert
```

Only perform these commands locally if this document was not already committed remotely; avoid duplicate commits. Open a PR for reviewed integration into `main` when appropriate.

## 10. Official reference material

- [Unreal Neural Network Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/neural-network-engine-in-unreal-engine)
- [Unreal Artificial Intelligence](https://dev.epicgames.com/documentation/en-us/unreal-engine/artificial-intelligence-in-unreal-engine)
- [Unreal C++ vs Blueprint](https://dev.epicgames.com/documentation/en-us/unreal-engine/coding-in-unreal-engine-blueprint-vs-cplusplus)
- [PyTorch ONNX exporter](https://docs.pytorch.org/docs/stable/onnx.html)
- [pytest](https://docs.pytest.org/)
- [ONNX Runtime Python](https://onnxruntime.ai/docs/get-started/with-python.html)

**Memory aid:** **Test → Train → Evaluate → Export → Verify → Integrate → Act.** Training is not inference; observations are not remembered history; the model chooses the action, and Unreal executes it.
