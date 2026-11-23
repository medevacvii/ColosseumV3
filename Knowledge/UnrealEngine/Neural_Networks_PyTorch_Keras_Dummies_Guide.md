# Neural Networks from Scratch — Complete Dummies Guide (PyTorch and Keras 3)

**ColosseumV3 · Beginner-friendly · Practical examples · Test-driven development (TDD)**

> Companion guides: [Start Without Existing Data](Creating_First_Neural_Network_Without_Data_Dummies_Guide.md) · [Activation Functions](Neural_Network_Activation_Functions_Dummies_Guide.md) · [AI → C++ → Unreal Architecture and TDD](AI_Model_Development_CPP_Unreal_TDD_Guide.md) · [Full Model Creation / ONNX Tutorial](AI_Model_Creation_Hands_On_TDD.md).
>
> **Status:** Teaching documentation. Code snippets are instructions for local exercises, **not** a claim that any code has been executed or models trained. Do not change production Unreal binary assets while learning.

## 1. The ten-second explanation

A **neural network** is a mathematical function with many adjustable numbers (**weights** and **biases**). It receives inputs, computes outputs and learns to improve those outputs by changing its weights during training.

For ColosseumV3:

```text
Unreal context -> [health, enemy attacking, surrendered, relationship ...]
              -> neural network -> action scores -> candidate action
              -> deterministic gameplay validator -> C++/Blueprint action
```

- **Training:** change weights using examples, feedback, or rewards.
- **Inference:** ask the already-trained model for a prediction.
- **Model checkpoint:** saved weights and associated versioned architecture/schema.
- **Context awareness:** provide fresh observations, including information looked up from character memory. This does NOT require retraining on every game start.
- **Identity:** Unreal already knows which Actor is perceived. Persistent character IDs locate per-person history; do not treat an arbitrary numeric ID as a meaningful continuous model feature.
- **Unknown:** an unidentified character must remain unknown; do not invent their identity.

## 2. What is a neuron? The arithmetic

For inputs `x1,x2`, weights `w1,w2`, and bias `b`:

```text
z = x1*w1 + x2*w2 + b
output = activation(z)
```

Example: health `0.2`, enemy threat `0.9`, weights `-1.2` and `1.8`, bias `-0.3`:

```text
z = (0.2 * -1.2) + (0.9 * 1.8) - 0.3 = 1.08
ReLU(z) = 1.08
```

A **layer** contains neurons. An **MLP** (multilayer perceptron) has an input layer, one or more hidden layers, and an output layer.

```text
4 features -> 16 hidden neurons -> 16 hidden neurons -> 3 action scores
```

The final scores are typically raw **logits**, not calibrated probabilities. `argmax` gives the highest-scoring action. `softmax` can produce numbers summing to one for inspection, but they should not be assumed perfectly calibrated.

## 3. Training vocabulary and memory aids

| Word | Meaning |
|---|---|
| Dataset | Examples the model learns from or is evaluated on |
| Label | Correct answer supplied for supervised training |
| Feature | A numerical input field, e.g. current health |
| Weight / bias | Parameters learned by the network |
| Activation | Nonlinear function between layers, e.g. ReLU |
| Forward pass | Compute predictions from inputs |
| Loss | Numerical measure of prediction error |
| Gradient | Direction/sensitivity of loss with respect to parameters |
| Backpropagation | Efficient computation of gradients through network |
| Optimizer | Updates parameters using gradients (e.g. Adam) |
| Learning rate | Controls optimizer's step size |
| Batch | Subset of examples used in an update |
| Epoch | One pass through the training data |
| Overfitting | Memorizes training examples; weak on new situations |
| Underfitting | Cannot learn enough even on training data |
| Validation set | Used to select hyperparameters/checkpoints |
| Test set | Untouched until final evaluation |
| ONNX | Portable neural-network format for inference |

**Mnemonic:** **Observe → Predict → Measure loss → Backpropagate → Update → Repeat.**

## 4. Why activations matter

Multiple plain linear layers collapse into one linear transformation. Activation functions like **ReLU**, **Leaky ReLU**, **GELU** or **SiLU** add nonlinearity, allowing a network to learn more complicated boundaries.

The correct **output** activation depends on the task:
- Exactly one action from several: raw logits + categorical cross-entropy (in PyTorch, `CrossEntropyLoss`).
- Independent yes/no flags: raw logits + `BCEWithLogitsLoss` (or a sigmoid-based output with a compatible loss).
- Regression of unrestricted values: linear output.

For detailed mathematical shapes, advantages, drawbacks and code examples, see the [Activation Functions Guide](Neural_Network_Activation_Functions_Dummies_Guide.md).

## 5. PyTorch vs Keras 3: what are they?

**PyTorch** is a deep-learning framework with tensors, automatic differentiation, modules and optimizers.

**Keras 3** is a high-level neural-network API supporting **PyTorch, TensorFlow and JAX backends**. Keras 3 using its PyTorch backend is still running computations on PyTorch, so this is not simply a battle between two entirely different numerical engines.

| Topic | PyTorch | Keras 3 |
|---|---|---|
| First model | More lines and concepts | Shorter `Sequential`, `compile`, `fit` |
| Custom training | Very flexible, explicit loop | Supports custom loops but extra code required |
| Debugging gradients | Direct access to operations | High-level abstraction can hide details |
| Research/novel reinforcement learning | Strong, fine-grained control | Possible, but backend/customization complexity |
| Typical supervised classifier | Good | Very convenient |
| Multiple backend experimentation | Primarily PyTorch | PyTorch / TF / JAX, subject to compatibility |
| Unreal deployment path | PyTorch -> ONNX -> Unreal NNE | Backend-specific export route -> ONNX/NNE verification |
| Common weaknesses | More boilerplate, easy to make loop errors | Abstraction, backend-dependent compatibility/export |

**Recommendation for this project:** learn a small model with Keras to see the idea quickly; implement the **primary Colosseum training/export pipeline in PyTorch** for fine control and explicit ONNX testing. Neither guarantees better accuracy merely by being the chosen framework.

## 6. Build the same tiny gladiator decision model

For a contained learning example, use **four inputs**:
1. target is hostile (0/1)
2. target is attacking (0/1)
3. target has surrendered (0/1)
4. my health (0.0 to 1.0)

Output labels: `0=OBSERVE`, `1=DEFEND`, `2=RETREAT`.

**Critical game rule:** surrender prohibits aggressive action. The final game should enforce restrictions as deterministic policy outside of neural inference, not trust a model to obey them in every case.

### 6A. Create a practice project (PyCharm / PowerShell)

```text
NeuralNetworkTutorial/
├── data.py
├── pytorch_model.py
├── keras_model.py
├── tests/
│   ├── test_data.py
│   ├── test_pytorch_model.py
│   └── test_keras_model.py
└── models/
```

Open **PyCharm → Terminal** at the practice directory. Paste commands **without copying the terminal prompt**:

```powershell
py -3.13 -m venv .venv
.\.venv\Scripts\python.exe -m pip install --upgrade pip
.\.venv\Scripts\python.exe -m pip install torch keras numpy pytest
.\.venv\Scripts\python.exe -m pytest --version
```

Use the currently supported installed Python version if 3.13 is unavailable. Keras below uses PyTorch as backend, so TensorFlow is not required for this lesson.

### 6B. TDD first: test your data

Write `tests/test_data.py` **before** writing `data.py`:

```python
import numpy as np
from data import create_dataset

def test_dataset_shape_and_range():
    x, y = create_dataset()
    assert x.shape == (1200, 4)
    assert y.shape == (1200,)
    assert np.isfinite(x).all()
    assert ((x >= 0) & (x <= 1)).all()
    assert set(np.unique(y)) == {0, 1, 2}

def test_data_generation_is_reproducible():
    x1, y1 = create_dataset(seed=120)
    x2, y2 = create_dataset(seed=120)
    np.testing.assert_array_equal(x1, x2)
    np.testing.assert_array_equal(y1, y2)
```

Run `.\.venv\Scripts\python.exe -m pytest tests/test_data.py -v`; verify RED. Then create `data.py`:

```python
import numpy as np

def create_dataset(n=1200, seed=120):
    rng = np.random.default_rng(seed)
    x = np.empty((n, 4), dtype=np.float32)
    x[:, 0] = rng.integers(0, 2, size=n)  # hostile
    x[:, 1] = rng.integers(0, 2, size=n)  # attacking
    x[:, 2] = rng.integers(0, 2, size=n)  # surrendered
    x[:, 3] = rng.uniform(0, 1, size=n)   # own health

    y = np.zeros(n, dtype=np.int64)       # OBSERVE
    active_threat = (x[:, 0] == 1) & (x[:, 1] == 1) & (x[:, 2] == 0)
    y[active_threat] = 1                  # DEFEND
    y[active_threat & (x[:, 3] < 0.20)] = 2  # RETREAT
    return x, y
```

Rerun pytest; verify GREEN. These are **synthetic labels derived from rules**, not learned combat wisdom. Random observations may contain contradictory flags, and actual game data will need consistency checks.

### 6C. PyTorch model and its TDD tests

Write `tests/test_pytorch_model.py` before implementation:

```python
import torch
from pytorch_model import GladiatorBrain

def test_pytorch_model_contract():
    logits = GladiatorBrain()(torch.zeros((2, 4)))
    assert logits.shape == (2, 3)
    assert torch.isfinite(logits).all()
```

Run RED; create `pytorch_model.py`:

```python
from torch import nn

class GladiatorBrain(nn.Module):
    def __init__(self):
        super().__init__()
        self.network = nn.Sequential(
            nn.Linear(4, 16),
            nn.ReLU(),
            nn.Linear(16, 16),
            nn.ReLU(),
            nn.Linear(16, 3),  # raw logits
        )
    def forward(self, x):
        return self.network(x)
```

Run GREEN.

In PyTorch the core training step is **explicit**:

```python
import torch
from data import create_dataset
from pytorch_model import GladiatorBrain

x_np, y_np = create_dataset()
x = torch.from_numpy(x_np)
y = torch.from_numpy(y_np)
model = GladiatorBrain()
optimizer = torch.optim.Adam(model.parameters(), lr=0.003)
loss_fn = torch.nn.CrossEntropyLoss()

model.train()
optimizer.zero_grad()
logits = model(x)
loss = loss_fn(logits, y)
loss.backward()
optimizer.step()
```

This snippet demonstrates **one optimizer step**, not a full trained model. For proper training, use a full pipeline with held-out splits, evaluation and checkpointing from [Hands-On AI Model Creation](AI_Model_Creation_Hands_On_TDD.md).

### 6D. Keras 3 model and TDD

Set `KERAS_BACKEND` **before any import of keras**, including imports triggered by test modules. Use a separate Python process for each backend choice.

Write `tests/test_keras_model.py` first:

```python
import os
os.environ["KERAS_BACKEND"] = "torch"
import numpy as np
from keras_model import build_model

def test_keras_model_contract():
    output = build_model().predict(
        np.zeros((2, 4), dtype=np.float32), verbose=0
    )
    assert output.shape == (2, 3)
    assert np.isfinite(output).all()
```

Run RED, then create `keras_model.py`:

```python
import os
os.environ["KERAS_BACKEND"] = "torch"  # before importing keras
import keras
from keras import layers

def build_model():
    model = keras.Sequential([
        keras.Input(shape=(4,)),
        layers.Dense(16, activation="relu"),
        layers.Dense(16, activation="relu"),
        layers.Dense(3),                 # logits
    ])
    model.compile(
        optimizer=keras.optimizers.Adam(learning_rate=0.003),
        loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
        metrics=["accuracy"],
    )
    return model
```

Run GREEN. For an easy supervised-learning fit:

```python
from data import create_dataset
from keras_model import build_model

x, y = create_dataset()
model = build_model()
history = model.fit(
    x[:840], y[:840],
    validation_data=(x[840:1020], y[840:1020]),
    epochs=50,
    batch_size=32,
    callbacks=[
        __import__("keras").callbacks.EarlyStopping(
            monitor="val_loss", patience=10, restore_best_weights=True
        )
    ],
    verbose=1,
)
test_loss, test_accuracy = model.evaluate(x[1020:], y[1020:], verbose=0)
print("Held-out test accuracy:", test_accuracy)
model.save("models/gladiator_keras.keras")  # create models/ first
```

**Note:** Create the `models` directory before calling `model.save`. In a real comparison, shuffle with a fixed seed, check group independence, and use identical partitions for both models. The two tiny demonstrations intentionally focus on clarity rather than a controlled benchmark.

## 7. Understand overfitting and underfitting before deployment

| Pattern | Interpretation |
|---|---|
| Train 55%, validation 54% | Likely underfitting |
| Train 91%, validation 89% | Promising generalization |
| Train 99%, validation 72% | Possible overfitting |

These are illustrative, NOT measured results.

- **Training set:** model adjusts its weights here.
- **Validation set:** decide training duration and model/hyperparameters.
- **Test set:** evaluate final choice only; don't repeatedly tune to it.
- Do not randomly mix **rows from the same encounter** across training and validation.
- Add **held-out-character / held-out-pair** evaluation when character identities matter.
- For each role/status, report recall and error cases; a strong overall score can hide dangerous rare failures.
- Use early stopping, smaller networks, weight decay and more varied encounters as needed.
- Deliberately fitting a tiny batch can be a helpful debugging test, but it is **not** evidence of generalization.

## 8. TDD and model-quality gates are different

**RED → GREEN → REFACTOR** for software contracts:
- Does feature encoding have correct dimensions and ordering?
- Are status flags valid and numerically finite?
- Does a forward pass produce `[batch, classes]`?
- Do gradients flow / weights update?
- Can model weights save and reload?
- Are ONNX and PyTorch outputs close within numerical tolerance?

**Train → Validate → Evaluate** for learned behaviour:
- Does validation loss fall?
- Does training substantially outperform validation?
- Are all important action classes recalled well?
- Does the policy generalize to new individuals and changed status?
- Is a test set held out until final evaluation?
- Are hard gameplay restrictions independently enforced?

The acceptance thresholds must be decided **before** using the evaluation set. A neural model that passes code tests can still make poor decisions.

## 9. How to compare PyTorch and Keras fairly

Keep the same feature schema, data split, 4→16→16→3 architecture, loss, optimizer type, learning rate, batch size, maximum epochs, stopping rule, hardware and several recorded random seeds. Compare validation/test accuracy, per-action metrics, training time, inference time and deployment compatibility. Expect stochastic variation; do not interpret one single run as definitive proof.

**Decision:** Prefer **PyTorch** as the primary Colosseum pipeline for controllable training and ONNX export; **Keras** remains excellent for understanding standard networks rapidly and trying backend-independent APIs where supported.

## 10. ONNX and Unreal integration

PyTorch weights (`.pt`) are not directly executable Unreal neural-network assets. Save the model and export a trained copy to ONNX:

```python
import torch
from pytorch_model import GladiatorBrain
m = GladiatorBrain()
m.load_state_dict(
    torch.load("models/gladiator_pytorch.pt", map_location="cpu", weights_only=True)
)
m.eval()
torch.onnx.export(
    m, (torch.zeros((1, 4)),), "models/gladiator.onnx",
    input_names=["observation"],
    output_names=["action_logits"],
    dynamo=True,
)
```

The code above assumes the model checkpoint `models/gladiator_pytorch.pt` already exists. Install `onnx`, `onnxscript`, and `onnxruntime`, and test numerical parity before importing to Unreal.

Unreal NNE runtime compatibility, tensor shapes, model-data import and platform support must be confirmed against the team's **installed Unreal Engine version**. The C++ adapter should run inference through an interface. Gameplay validation and Blueprint/StateTree perform the allowed action. Save/travel/restart should **not** retrain the model.

## 11. Suggested mini-sprints

1. **Math:** weighted sum, bias, ReLU, simple manual calculations.
2. **Data:** RED/GREEN tests, synthetic situations and labels.
3. **PyTorch:** build MLP, test shape/gradients, train with separate validation.
4. **Keras:** recreate architecture, train with `fit()`, compare.
5. **Generalization:** fix leakage, test unseen characters, handle surrender correctly.
6. **Export:** PyTorch checkpoint → ONNX → numerical parity.
7. **Unreal:** NNE adapter → C++ tests → Blueprint functional tests.

## 12. References

- [PyTorch Learn the Basics](https://docs.pytorch.org/tutorials/beginner/basics/intro.html)
- [PyTorch Get Started](https://pytorch.org/get-started/locally/)
- [Keras 3 multi-backend API](https://keras.io/keras_3/)
- [Keras Sequential model](https://keras.io/guides/sequential_model/)
- [Keras training with built-in methods](https://keras.io/guides/training_with_built_in_methods/)
- [PyTorch ONNX exporter](https://docs.pytorch.org/docs/stable/onnx.html)
- [Unreal Neural Network Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/neural-network-engine-in-unreal-engine)

**Memory aid: Inputs → Weights → Activation → Prediction → Loss → Backprop → Optimizer.**
