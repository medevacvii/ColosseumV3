# ColosseumV3 — AI development instructions

These instructions apply repository-wide on the AI-model branch. Preserve existing team assets and more specific nested instructions. Do not expand approved scope.

## Commit messages
Use the canonical [root commit template](.gitmessage.txt), adapted from ParkingPatrol. Use type(scope): imperative summary (ideally <=72 characters). Record why, actual changes, RED/GREEN/REFACTOR results, documentation, model/integration validation, privacy/security, AI assistance, sprint/story and verified issue references. Remove placeholders; never invent tests or review. State Not run or N/A honestly.

Configure once per local clone from the repository root:
```powershell
git config --local commit.template .gitmessage.txt
git config --local --get commit.template
```
Use git commit without -m to open the template. For IDE-generated messages (PyCharm, Rider, Visual Studio), supply the template as context and review the staged diff and generated message. GitHub web/API commits do not automatically use local config: explicitly supply the complete applicable message. Never include secrets, personal information or restricted content in commit history.

## Mandatory agent issue lookup and linking
1. Before work/commit, identify the approved sprint/story from [Scrum schedule](Knowledge/UnrealEngine/AI_MODEL_SCRUM_SCHEDULE.md). Search both open and closed GitHub issues for relevant scope, state and acceptance criteria; confirm actual issue numbers.
2. Reuse matching issues. If none exists, create one only when authorized; otherwise flag missing issue and stop before committing. The initial commit-system bootstrap is the sole exception: the all-state issue lookup returned no issues and the setup is recorded as N/A.
3. The agent supplies references automatically; never make the developer look up issue numbers. Use Refs #N for partial, pending-review or open work. Use Closes #N only when the entire issue is verified and explicit product owner reading/acceptance is recorded and closure intended. Do not close a grouped issue early. Closing keywords may close issues when commits reach the default branch.
4. Include references in API commit message bodies and PR descriptions. After committing, verify commit SHA/message and report issue links, actual test results, and AI-use/review status.
5. A matching issue is not approval to expand scope. Do not merge to main without authorization.

## TDD and documentation
Write tests first for deterministic behavior, observe RED, implement GREEN, refactor and rerun regression tests. For Python use PEP 257 and Google-style docstrings with actual design pattern, rationale, typical usage, public API arguments/returns/exceptions as applicable. For C++ document public interfaces, ownership, lifecycle, Unreal reflection and actual pattern responsibilities. Preserve executable behavior in documentation-only work. Do not invent pattern usage.
For ML record seeds, dataset/model/schema versions, grouped splits, leakage checks, validation metrics and overfitting checks. Keep final test data separate from tuning. Verify ONNX parity and Unreal runtime compatibility before deployment; use a safe fallback. Never claim tests passed without observed evidence.

## AI transparency and review
Record actual tool/model when known, contributions, AI-use log ID if established (otherwise N/A), and product owner acceptance status. Do not invent approvals. Establish a versioned AI-use log during S01. Work in small approved steps, demonstrate results and obtain acceptance before advancing sprint scope. Sundays remain rest days.

## Unreal teamwork
Use isolated test maps, coordinate Git LFS locks for binary assets, avoid unrelated .uasset/.umap changes, and maintain high cohesion and low coupling between C++, Blueprint and model. Keep true hidden opponent statistics separate from perception-based inference. Do not commit credentials or unapproved generated data/models.
