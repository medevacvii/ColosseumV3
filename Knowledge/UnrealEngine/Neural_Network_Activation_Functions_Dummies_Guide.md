# Neural-Network Activation Functions — Complete Dummies Guide

**ColosseumV3 · PyTorch + Keras 3 · Mathematics, strengths, weaknesses and TDD**

> Read together with [Neural Networks from Scratch](Neural_Networks_PyTorch_Keras_Dummies_Guide.md), [Create a Model with No Existing Data](Creating_First_Neural_Network_Without_Data_Dummies_Guide.md), and the [Unreal C++ / AI TDD Guide](AI_Model_Development_CPP_Unreal_TDD_Guide.md).
>
> **Status:** Documentation only; no implementation, training or Unreal asset changes are implied.

## 1. What is an activation function?

A neural-network neuron calculates a weighted sum, adds bias and transforms the result through an **activation function**:

```text
weighted_sum = x1*w1 + x2*w2 + ... + b
neuron_output = activation(weighted_sum)
```

The key idea is **nonlinearity**. If you stack only linear transformations, they collapse to another linear transformation, so adding layers alone cannot model complex nonlinear decision boundaries.

**Two locations matter:**
- **Hidden layer activation:** affects learned internal features (ReLU, Leaky ReLU, GELU, SiLU and others).
- **Output activation:** depends on the problem: one class, multiple independent classes/flags, or unrestricted continuous values.

**Never select the output activation without also checking the loss function.**

## 2. ReLU — Rectified Linear Unit

**Formula:** `ReLU(x) = max(0, x)`

| x | ReLU(x) |
|---:|---:|
| -10 | 0 |
| -2 | 0 |
| 0 | 0 |
| 2 | 2 |
| 10 | 10 |

**Strengths:** simple, fast, sparse activations, commonly effective for MLPs and CNNs.

**Weaknesses:** derivative is zero when x < 0; a neuron that stays in that region can stop updating (the **dying ReLU** problem). It is not differentiable at x = 0, where frameworks choose a usable subgradient.

**Use it for:** starting Colosseum structured-data MLPs.

```python
# PyTorch
import torch.nn as nn
hidden_activation = nn.ReLU()

# Keras
from keras import layers
hidden_activation = layers.Activation("relu")
```

## 3. Leaky ReLU

**Formula:** `LeakyReLU(x) = x if x >= 0 else alpha*x`

Example with `alpha=0.1`: `-2 -> -0.2`, `2 -> 2`.

**Strengths:** allows nonzero negative-side gradient, reducing the classical dying-ReLU concern.

**Weaknesses:** introduces slope hyperparameter; may not improve performance and can reduce sparsity.

```python
# PyTorch
nn.LeakyReLU(negative_slope=0.01)

# Keras
layers.LeakyReLU(negative_slope=0.01)
```

## 4. PReLU — Parametric ReLU

Like Leaky ReLU, but the negative slope is **learned** rather than fixed.

**Strengths:** adapts negative activation behaviour to the dataset.

**Weaknesses:** extra parameters and potential overfitting; less suitable when simplicity is the priority.

```python
# PyTorch
nn.PReLU()

# Keras
layers.PReLU()
```

## 5. ELU — Exponential Linear Unit

**Formula:** `ELU(x)=x` if positive; otherwise `alpha*(exp(x)-1)`.

**Strengths:** smooth negative branch (except possible slope discontinuity with certain alpha), negative-valued activations, sometimes helpful optimization.

**Weaknesses:** requires exponentials; derivative becomes small for large negative inputs; not universally better than ReLU.

```python
nn.ELU(alpha=1.0)            # PyTorch
layers.ELU(alpha=1.0)        # Keras
```

## 6. SELU — Scaled ELU

A specially scaled ELU that supports **self-normalizing neural networks under certain architectural assumptions**.

**Strengths:** activation distribution can remain relatively stable when assumptions hold.

**Weaknesses:** depends on compatible initialization (typically LeCun normal), suitable network structure and AlphaDropout for dropout; not a universal replacement for ReLU.

```python
nn.SELU()                         # PyTorch
layers.Activation("selu")         # Keras
```

## 7. GELU — Gaussian Error Linear Unit

**Formula:** `GELU(x)=x * Phi(x)`, where `Phi` is the standard normal CDF.

**Strengths:** smooth gating; popular in transformers and many sophisticated architectures.

**Weaknesses:** heavier computation than ReLU and not automatically superior for a small MLP.

```python
nn.GELU()                         # PyTorch
layers.Activation("gelu")         # Keras
```

## 8. SiLU / Swish

**Formula:** `SiLU(x)=x*sigmoid(x)`.

**Strengths:** smooth, non-monotonic, retains small negative activations; performs well in many modern models.

**Weaknesses:** more expensive than ReLU; may provide no measurable improvement for tiny MLPs.

```python
nn.SiLU()                         # PyTorch
layers.Activation("silu")         # Keras
```

## 9. Sigmoid / logistic

**Formula:** `sigmoid(x)=1/(1+exp(-x))`.

Output lies **strictly between 0 and 1**.

**Strengths:** useful for probabilities of one binary outcome or several **independent** labels.

**Weaknesses:** saturates at large positive/negative inputs; gradients can vanish, and hidden-layer training can be slow.

**Output warning:**
- PyTorch binary classification: prefer raw logits + `nn.BCEWithLogitsLoss()` (numerically stable).
- Keras: prefer raw logits + `BinaryCrossentropy(from_logits=True)`; or explicitly output sigmoid and use `from_logits=False`.

```python
nn.Sigmoid()                                # PyTorch
layers.Activation("sigmoid")                # Keras
```

## 10. Tanh — hyperbolic tangent

**Formula:** `tanh(x)=(exp(x)-exp(-x))/(exp(x)+exp(-x))`.

Output is between **-1 and +1**; zero-centered.

**Strengths:** useful when you need signed bounded outputs or certain recurrent-network structures.

**Weaknesses:** saturation and vanishing gradients for large input magnitudes.

```python
nn.Tanh()                              # PyTorch
layers.Activation("tanh")              # Keras
```

## 11. Softplus

**Formula:** `softplus(x)=log(1+exp(x))`.

A smooth approximation of ReLU.

**Strengths:** smooth positive output, nonzero gradients for finite inputs; useful for some positive-valued parameters.

**Weaknesses:** less sparse, more expensive than ReLU; does not automatically improve classifiers.

```python
nn.Softplus()                           # PyTorch
layers.Activation("softplus")           # Keras
```

## 12. Softmax

For logits `z_i`, `softmax(z)_i = exp(z_i) / sum_j exp(z_j)` (implemented with numerical stabilization).

**Strengths:** converts **mutually exclusive** class scores into nonnegative values summing to 1.

**Weaknesses:** not appropriate for **independent multi-label** outputs; may give overconfident probabilities unless calibrated. Do not apply it before PyTorch `CrossEntropyLoss`, which expects logits.

Example Colosseum action scores:

```text
Raw logits:  [1.4, 2.5, 0.2]
             OBSERVE DEFEND RETREAT
Softmax:     three scores between 0 and 1, summing to 1
Decision:    highest score -> DEFEND
```

```python
# PyTorch: inference display only
import torch
scores = torch.tensor([[1.4, 2.5, 0.2]])
probabilities = torch.softmax(scores, dim=1)

# Keras: inference display only
from keras import ops
probabilities_keras = ops.softmax(scores, axis=-1)
```

During categorical-classification training, use the correct logits-based cross-entropy.

## 13. Linear / Identity

**Formula:** `f(x)=x`.

**Strengths:** unrestricted numeric outputs, useful for predicting damage amounts or other regression quantities.

**Weaknesses:** no nonlinear transformation; stacks of only linear layers are equivalent to one linear mapping.

```python
nn.Identity()                           # PyTorch
layers.Activation("linear")             # Keras
```

## 14. Other names worth knowing

- **Hard Sigmoid / Hard Swish:** cheaper piecewise approximations sometimes useful on constrained inference targets.
- **Mish:** `x*tanh(softplus(x))`, smooth and non-monotonic; compute cost can exceed ReLU without a guaranteed gain.
- **GLU / gated activations:** split hidden representations into a signal and a gate; common in certain sequence and transformer architectures. Must plan dimensions carefully.
- **Recurrent gating:** sigmoid and tanh still matter in LSTMs/GRUs, even when not the default hidden MLP activation.

## 15. One-page strengths-and-weaknesses comparison

| Function | Typical use | Strength | Weakness |
|---|---|---|---|
| ReLU | MLP/CNN hidden layers | Cheap and simple | Dying ReLU |
| Leaky ReLU | Hidden layers | Gradient on negative side | Fixed slope hyperparameter |
| PReLU | Hidden layers | Learns negative slope | More parameters |
| ELU | Hidden layers | Smooth negative branch | Exponential/saturation |
| SELU | Self-normalizing architectures | Stable activations under assumptions | Strict architectural considerations |
| GELU | Transformers, hidden layers | Smooth gating | More compute |
| SiLU | Modern hidden layers | Smooth, retains negatives | More compute |
| Sigmoid | Binary output | 0–1 range | Gradient saturation |
| Tanh | Signed output/recurrent | -1 to +1, zero-centered | Gradient saturation |
| Softplus | Smooth positive outputs | Smooth and positive | More expensive than ReLU |
| Softmax | One-of-N probabilities | Sums to one | Wrong for independent labels |
| Linear | Regression output | Unrestricted output | No extra nonlinearity |

## 16. Choose activation based on the prediction target

| What are you predicting? | Recommended output | Compatible training loss |
|---|---|---|
| One action among OBSERVE/DEFEND/RETREAT | **Raw logits**, softmax only for inspection | PyTorch `CrossEntropyLoss`; Keras `SparseCategoricalCrossentropy(from_logits=True)` |
| Independent statuses: injured / armed / hostile | Raw logits, sigmoid at inference | PyTorch `BCEWithLogitsLoss`; Keras `BinaryCrossentropy(from_logits=True)` |
| Signed movement steering in [-1,1] | Tanh | Task-specific regression loss |
| Continuous damage / position | Linear | MSE or other regression loss |
| Strictly positive parameter | Softplus where appropriate | Depends on task |

**Unreal design:** action scores belong in the model; gameplay rules, target selection and animations belong outside it. Explicitly prohibit invalid or disallowed actions with a deterministic validator.

## 17. Hands-on PyTorch comparison

Create an isolated practice script:

```python
import torch
from torch import nn

values = torch.tensor([-3.0, -1.0, 0.0, 1.0, 3.0])
functions = {
    "ReLU": nn.ReLU(),
    "LeakyReLU": nn.LeakyReLU(negative_slope=0.1),
    "GELU": nn.GELU(),
    "SiLU": nn.SiLU(),
    "Sigmoid": nn.Sigmoid(),
    "Tanh": nn.Tanh(),
    "Softplus": nn.Softplus(),
}

for name, function in functions.items():
    print(name, [round(float(x), 4) for x in function(values)])
```

Observe what happens for negative, zero and positive inputs. This is a **mathematical demonstration**, not trained model behaviour.

## 18. Test-driven development: RED → GREEN → REFACTOR

Write tests **before** any custom activation wrapper, model switcher or optimizer experiment. Built-in PyTorch activations are already library-tested; your tests primarily protect **your own contracts and integrations**.

```python
# tests/test_activations.py
import torch
import pytest
from torch import nn

def test_relu_clips_negative_values():
    x = torch.tensor([-2.0, 0.0, 3.0])
    assert nn.ReLU()(x).tolist() == [0.0, 0.0, 3.0]

def test_leaky_relu_retains_negative_slope():
    result = nn.LeakyReLU(negative_slope=0.1)(torch.tensor([-2.0]))
    torch.testing.assert_close(result, torch.tensor([-0.2]))

@pytest.mark.parametrize("activation", [nn.ReLU(), nn.LeakyReLU(), nn.GELU(), nn.SiLU()])
def test_hidden_activation_preserves_shape_and_finiteness(activation):
    x = torch.tensor([-2.0, 0.0, 2.0])
    y = activation(x)
    assert y.shape == x.shape
    assert torch.isfinite(y).all()

def test_softmax_probabilities_sum_to_one():
    logits = torch.tensor([[1.4, 2.5, 0.2]])
    probabilities = torch.softmax(logits, dim=1)
    torch.testing.assert_close(probabilities.sum(dim=1), torch.ones(1))
```

Run from the practice project root:

```powershell
.\.venv\Scripts\python.exe -m pytest tests/test_activations.py -v
```

These tests should **pass immediately** with correctly installed PyTorch. To experience real RED → GREEN, create a failing test for *your own* planned `build_model(activation_name)` function and implement the selectable activation only after seeing it fail.

## 19. Keras equivalents

Set backend before importing Keras:

```python
import os
os.environ["KERAS_BACKEND"] = "torch"
from keras import layers

activations = {
    "relu": layers.Activation("relu"),
    "leaky_relu": layers.LeakyReLU(negative_slope=0.1),
    "gelu": layers.Activation("gelu"),
    "silu": layers.Activation("silu"),
    "sigmoid": layers.Activation("sigmoid"),
    "tanh": layers.Activation("tanh"),
}
```

For a Keras MLP:

```python
import keras
from keras import layers

model = keras.Sequential([
    keras.Input(shape=(4,)),
    layers.Dense(16, activation="relu"),
    layers.Dense(16, activation="relu"),
    layers.Dense(3),  # raw logits; NOT softmax here
])
model.compile(
    optimizer="adam",
    loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
    metrics=["accuracy"],
)
```

## 20. Compare activations scientifically, not by popularity

For Colosseum start with **ReLU**; compare **Leaky ReLU** and **SiLU** if data suggest a problem. Use:
1. Same model input/output contract and action mapping.
2. Same training/validation/test encounter groups.
3. Same initial hidden dimensions and optimizer settings.
4. Multiple controlled random seeds; compare distribution of results, not just one run.
5. Training/validation loss and class-specific metrics; check **overfitting** and data leakage.
6. CPU inference timing and Unreal ONNX/NNE compatibility.
7. Dedicated regression tests for surrendered, unknown and changing-status characters.

A tiny neural network with a good dataset can outperform a complicated network with poor data. A smoother or newer activation is **not automatically better**.

## 21. Frequently confused topics

**Q: Is ReLU a neuron?** No. It is a function applied to the neuron's weighted sum.

**Q: Does ReLU decide ATTACK?** No. The entire model outputs action scores; ReLU is only part of internal computation.

**Q: Does Softmax train the model?** No. It converts class scores to normalized values. Training requires labels, loss, backpropagation and optimization (or another valid learning method).

**Q: Why is Softmax not at the end of the PyTorch training model?** `CrossEntropyLoss` expects logits and handles the normalization mathematically with better numerical stability.

**Q: Do I need to retrain to switch from ReLU to SiLU?** Yes, for a valid comparison you should retrain after changing the hidden-layer activation. A checkpoint's existing weights were optimized for the prior architecture.

**Q: Will Keras and PyTorch produce the same predictions?** Not automatically. Even if layer shapes match, random initialization, training order and implementation details can differ.

**Q: Will one activation prevent overfitting?** No. Use appropriate independent datasets, validation, regularization, early stopping and honest testing.

## 22. Recommended roadmap for ColosseumV3

1. Build the tiny `4→16→16→3` model using **ReLU**.
2. Use TDD to confirm features and logits shape.
3. Generate synthetic encounters, train and track validation.
4. Compare Leaky ReLU and SiLU only after establishing the ReLU baseline.
5. Expand to versioned character-role, status, relationship and memory features.
6. Verify PyTorch/ONNX parity and target NNE support.
7. Integrate behind a C++ decision-provider interface, then test Blueprint behaviour.

**Memory aid:** **ReLU = fast; Leaky ReLU = negatives leak; GELU/SiLU = smooth; Sigmoid = yes/no; Tanh = signed range; Softmax = one choice; Linear = unrestricted number.**

## 23. Official references

- [PyTorch activation modules](https://docs.pytorch.org/docs/stable/nn.html#non-linear-activations-weighted-sum-nonlinearity)
- [PyTorch CrossEntropyLoss](https://docs.pytorch.org/docs/stable/generated/torch.nn.CrossEntropyLoss.html)
- [PyTorch BCEWithLogitsLoss](https://docs.pytorch.org/docs/stable/generated/torch.nn.BCEWithLogitsLoss.html)
- [Keras activation functions](https://keras.io/api/layers/activations/)
- [Keras advanced activation layers](https://keras.io/api/layers/activation_layers/)
- [Unreal Neural Network Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/neural-network-engine-in-unreal-engine)
