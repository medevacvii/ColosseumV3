# Guided sprint sessions

Each story is a small guided session, not a fixed daily commitment. Work only after sprint scope approval. Sundays off; holiday pause Dec 21–Jan 3. See issue index for planned dates. For each session: explain what/why/where/how, inspect current code, write test first, observe RED, implement GREEN, refactor and run regression, show visible result, record AI help and obtain reading/acceptance; commit with actual issue reference. For ML use deterministic contract tests plus experiment gates; never fabricate test results.

## S01-01 — #2
As a developer, I want an isolated Python/PyTorch environment

Expected evidence: clean checkout installs pinned dependencies and verifies interpreter.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S01-02 — #2
As a developer, I want automated TDD

Expected evidence: show observed failing test, passing implementation and regression run.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S01-03 — #2
As a developer, I want independent modules

Expected evidence: document interfaces, responsibilities and actual patterns.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S01-04 — #2
As product owner, I want traceable development

Expected evidence: establish AI log and progress records with review gates.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S02-01 — #3
As a gladiator, I want actor recognition

Expected evidence: stable IDs; role, rank and relationship distinct.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S02-02 — #3
As a gladiator, I want combat stats

Expected evidence: define power semantics; reject invalid health, attack, defense and power.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S02-03 — #3
As a gladiator, I want social context

Expected evidence: test conversation capability, activity, alive and distance independently.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S02-04 — #3
As model developer, I want encoded features

Expected evidence: version schema; unknown differs from zero; confidence and age validated.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S03-01 — #3
As a gladiator, I want conversation eligibility

Expected evidence: reject dead or out-of-range participants.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S03-02 — #3
As a gladiator, I want combat policy

Expected evidence: test Talk/Attack/Defend/Retreat/Ignore baseline.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S03-03 — #3
As a gladiator, I want target selection

Expected evidence: deterministic ties and eligible targets only.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S03-04 — #3
As developer, I want legal action validation

Expected evidence: safe fallback when none eligible; demo two actors.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S04-01 — #4
As developer, I want synthetic actors

Expected evidence: valid ranges and deterministic seeds.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S04-02 — #4
As developer, I want diverse encounters

Expected evidence: include unknown features, conflicting evidence and varied roles.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S04-03 — #4
As model developer, I want labels

Expected evidence: check legal actions, document policy and class distribution.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S04-04 — #4
As researcher, I want independent splits

Expected evidence: grouped encounter/archetype separation; manifest and repeatable generation.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S05-01 — #5
As developer, I want a small MLP

Expected evidence: test input/output tensor shapes.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S05-02 — #5
As learner, I want activation/loss rationale

Expected evidence: finite loss and documented ReLU/classification choice.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S05-03 — #5
As developer, I want reproducible training

Expected evidence: controlled learning test and checkpoint reload.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S05-04 — #5
As gladiator, I want action scores

Expected evidence: test masking and held-out predictions.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S06-01 — #5
As researcher, I want metrics

Expected evidence: confusion matrix, macro F1 and per-action recall verified against examples.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S06-02 — #5
As developer, I want learning curves

Expected evidence: diagnose under/overfitting.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S06-03 — #5
As developer, I want tuning

Expected evidence: validation only; preprocessing fit on training only; grouped separation.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S06-04 — #5
As product owner, I want deployment gate

Expected evidence: predeclare thresholds, untouched test evaluation, baseline comparison and explicit approval; imitation is not superiority.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S07-01 — #6
As developer, I want ONNX export

Expected evidence: validate opset and versioned schema.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S07-02 — #6
As developer, I want inference parity

Expected evidence: compare PyTorch/ONNX within agreed tolerance.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S07-03 — #6
As Unreal developer, I want C++ adapter

Expected evidence: verify supported NNE runtime and reject schema mismatch.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S07-04 — #6
As player, I want fallback

Expected evidence: invalid model and input fail safely.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S08-01 — #6
As Blueprint developer, I want facade

Expected evidence: request and receive decision.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S08-02 — #6
As gladiator, I want context components

Expected evidence: missing components fail safely.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S08-03 — #6
As developer, I want executor

Expected evidence: invalid recommendations cannot bypass C++ validation.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S08-04 — #6
As tester, I want isolated demo

Expected evidence: two actors and diagnostics; unrelated assets unchanged.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S09-01 — #7
As gladiator, I want observations

Expected evidence: no new visual data when unobserved.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S09-02 — #7
As gladiator, I want estimated stats

Expected evidence: contradictory evidence revises beliefs.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S09-03 — #7
As gladiator, I want confidence and freshness

Expected evidence: validate per-stat age and unknown masks.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S09-04 — #7
As model developer, I want hybrid policy

Expected evidence: train on perceived features; hidden-stat changes alone cannot change deterministic inference.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S10-01 — #7
As gladiator, I want recognition

Expected evidence: stable actor ID retrieval.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S10-02 — #7
As gladiator, I want relationship memory

Expected evidence: independent trust/fear/familiarity per observer.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S10-03 — #7
As gladiator, I want aging

Expected evidence: stale estimates lose confidence.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S10-04 — #7
As player, I want save/load

Expected evidence: restart restores supported fields; weights unchanged by encounters.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S11-01 — #8
As gladiator, I want multiple candidates

Expected evidence: exclude ineligible partners.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S11-02 — #8
As gladiator, I want status-sensitive behavior

Expected evidence: test role and relationship independently.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S11-03 — #8
As player, I want personality differences

Expected evidence: approved courage/aggression/risk scenarios yield differentiated behavior.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S11-04 — #8
As gladiator, I want conversation intent

Expected evidence: greet/warn/negotiate/cooperate separate from dialogue generation.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S12-01 — #9
As developer, I want end-to-end regression

Expected evidence: Python/Unreal tests, save/load and fallback pass.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S12-02 — #9
As player, I want responsiveness

Expected evidence: measure agreed hardware-specific budget.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S12-03 — #9
As teammate, I want maintainable docs

Expected evidence: architecture, schema, setup and actual patterns reviewed.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded

## S12-04 — #9
As product owner, I want final demo

Expected evidence: VR/non-VR supported smoke tests and observation-to-memory demonstration; explicit acceptance.

- [ ] Scope approved
- [ ] Test/experiment evidence recorded
- [ ] Demo and owner acceptance recorded
