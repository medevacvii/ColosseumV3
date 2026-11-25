# ColosseumV3 — Master documentation index

Updated 2026-11-24 | Branch: AI-model | Status: pending product owner review
Governance: [Issue #1](https://github.com/medevacvii/ColosseumV3/issues/1)

## Start here
This is the project's library catalogue: use it to find the right guide, not as a substitute for the guide itself. It indexes the AI-model branch documentation, including the new system architecture specification. Other branches are outside this inventory.

**New to Unreal?** Start with the beginner guide. **New to AI?** Start with neural networks. **Joining the development team?** Read AGENTS, the backlog and the definition of done before changing anything. **Reviewing progress?** Open sprint progress and the issue index.

## 1. Unreal and Blueprint learning
| Document | What it teaches |
|---|---|
| [Unreal learning library](../README.md) | Choose a lesson and find a practice exercise |
| [UE5 beginner hands-on guide](../UE5_Beginners_Hands_On_Guide.md) | Editor and basic workflows |
| [Unreal Engine and Blueprint Bible](../Unreal_Engine_Blueprint_Bible.md) | Detailed reference, Blueprint logic and C++ integration |

## 2. AI learning and technical architecture
| Document | What it teaches |
|---|---|
| [Neural networks: PyTorch and Keras](../Neural_Networks_PyTorch_Keras_Dummies_Guide.md) | Neural network foundations and framework comparison |
| [Activation functions](../Neural_Network_Activation_Functions_Dummies_Guide.md) | ReLU and alternatives |
| [First neural network without data](../Creating_First_Neural_Network_Without_Data_Dummies_Guide.md) | Starting with synthetic examples |
| [Hands-on AI creation with TDD](../AI_Model_Creation_Hands_On_TDD.md) | Practical model development and tests |
| [AI development, C++, Unreal and TDD](../AI_Model_Development_CPP_Unreal_TDD_Guide.md) | Integration learning guide |
| [System architecture specification](SYSTEM_ARCHITECTURE_SPECIFICATION.md) | Proposed system-wide engineering blueprint and contracts |
| [Reflection context framework](REFLECTION_CONTEXT_FRAMEWORK.md) | Proposed architecture, interfaces, perception, memory, ONNX and validation |

## 3. Project planning and governance
| Document | When to use it |
|---|---|
| [Repository agent instructions](../../../AGENTS.md) | Before any development or commit |
| [Knowledge organization proposal](../../../KNOWLEDGE_ORGANIZATION_PROPOSAL.md) | Understand proposed documentation organization |
| [Scrum schedule](../AI_MODEL_SCRUM_SCHEDULE.md) | Check sprint timing and dependencies |
| [Product backlog](Planning/PRODUCT_BACKLOG.md) | Find the 48 approved stories and acceptance criteria |
| [GitHub issue workflow](Planning/GITHUB_ISSUE_WORKFLOW.md) | Find actual issue numbers and linking rules |
| [Sprint progress](Planning/SPRINT_PROGRESS.md) | Check accepted work and evidence |
| [Guided sprint sessions](Planning/GUIDED_SPRINT_SESSIONS.md) | Work through small learning and development steps |
| [Definition of done](Planning/DEFINITION_OF_DONE.md) | Determine whether work is ready for acceptance |
| [AI-use log](AI_USE_LOG.md) | Trace AI assistance and recorded approvals |
| [Commit template](../../../.gitmessage.txt) | Prepare traceable commit messages |

## 4. Reusable templates
| Template | Purpose |
|---|---|
| [User story](Templates/USER_STORY_TEMPLATE.md) | Describe a requirement and testable acceptance |
| [Sprint review](Templates/SPRINT_REVIEW_TEMPLATE.md) | Demonstrate results and record acceptance |
| [Sprint retrospective](Templates/SPRINT_RETROSPECTIVE_TEMPLATE.md) | Learn from the sprint and improve |

## 5. Choose a reading path
| Reader | Suggested order |
|---|---|
| Non-programmer / product owner | Reflection framework plain-English sections → backlog → sprint progress |
| Blueprint developer | UE5 beginner → Blueprint Bible → reflection framework → issue #6 |
| C++ developer | AGENTS → reflection framework → C++ integration guide → definition of done |
| AI learner | Neural networks → activation functions → first network → hands-on TDD |
| Tester | Definition of done → backlog acceptance criteria → reflection TDD examples → review template |

Read one section at a time. Try its smallest exercise in an isolated practice project when authorized. A learning example is not evidence of a production feature.

## 6. Sprint navigation
| Sprint | Issue | Documentation focus |
|---|---|---|
| S01 | [#2](https://github.com/medevacvii/ColosseumV3/issues/2) | Environment, TDD, contracts, governance |
| S02–S03 | [#3](https://github.com/medevacvii/ColosseumV3/issues/3) | Features, perception contract and rules |
| S04 | [#4](https://github.com/medevacvii/ColosseumV3/issues/4) | Synthetic data |
| S05–S06 | [#5](https://github.com/medevacvii/ColosseumV3/issues/5) | Training and evaluation |
| S07–S08 | [#6](https://github.com/medevacvii/ColosseumV3/issues/6) | ONNX and Unreal integration |
| S09–S10 | [#7](https://github.com/medevacvii/ColosseumV3/issues/7) | Perception and memory |
| S11 | [#8](https://github.com/medevacvii/ColosseumV3/issues/8) | Social intelligence |
| S12 | [#9](https://github.com/medevacvii/ColosseumV3/issues/9) | Validation and release |

The live progress document is authoritative for sprint status. This index does not authorize work or change dates.

## 7. Proposed documentation gaps
These are proposed future specifications, not approved new work or claims that existing guides omit every related topic. No links are supplied until files exist.

| Proposed document | Suggested timing | Related issue |
|---|---|---|
| [System architecture specification](SYSTEM_ARCHITECTURE_SPECIFICATION.md) — drafted, review pending | Before S01 | #1 |
| Architecture decision records and template | Before S01 | #1 |
| Team integration agreement | Before S01 | #1 |
| Comprehensive testing strategy | S01 | #2 |
| Roman historical and social behaviour specification | Before S02 | #3, #8 |
| Feature dictionary and perception/memory contract | S02 | #3, #7 |
| Decision-making specification | S02–S03 | #3 |
| Synthetic data specification | S04 | #4 |
| Model evaluation and model cards | S05–S06 | #5 |
| API reference and recovery guide | S07–S08 | #6 |
| Character memory specification | S09–S10 | #7 |
| Performance and scalability guide | S07–S12 | #6, #9 |
| Deployment and rollback guide | S12 | #9 |

Define perception and memory contracts early to reduce retraining risk; implementation remains subject to existing sprint approvals.

## 8. Living index maintenance
When an authorized documentation change adds, renames or removes a guide, update this index in the same change where practical. Check relative links against the target branch, avoid duplicate guides, and label proposed versus accepted specifications. Each guide should use plain English, terminology, examples, technical details, expected results, limitations and issue references as applicable. Record actual test evidence and acceptance; never infer acceptance from a commit. Preserve Sunday rest and holiday pause. Do not move files or modify other teams' assets merely to reorganize the catalogue.

AI contribution: ChatGPT GPT-6.2 drafted this index from the branch inventory and prior documentation assessment. Product owner review pending. No executable code or sprint implementation changed.
