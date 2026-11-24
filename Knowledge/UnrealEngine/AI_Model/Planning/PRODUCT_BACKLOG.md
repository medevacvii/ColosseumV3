# Product backlog

Approved scope; 48 stories, 12 sprints, eight development groups. Governance: [#1](https://github.com/medevacvii/ColosseumV3/issues/1). All stories unstarted; order follows dependencies. No implementation approval yet.

## [AI-01] Development environment and TDD foundation — [#2](https://github.com/medevacvii/ColosseumV3/issues/2)

Parent tracking: #1
Dependency: None. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S01 — Nov 30–Dec 13 2026
- [ ] S01-01: As a developer, I want an isolated Python/PyTorch environment
  - Acceptance/TDD: clean checkout installs pinned dependencies and verifies interpreter.
- [ ] S01-02: As a developer, I want automated TDD
  - Acceptance/TDD: show observed failing test, passing implementation and regression run.
- [ ] S01-03: As a developer, I want independent modules
  - Acceptance/TDD: document interfaces, responsibilities and actual patterns.
- [ ] S01-04: As product owner, I want traceable development
  - Acceptance/TDD: establish AI log and progress records with review gates.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-02] Actor features and rule-based intelligence — [#3](https://github.com/medevacvii/ColosseumV3/issues/3)

Parent tracking: #1
Dependency: #2 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S02 — Dec 14 2026–Jan 10 2027; pause Dec 21–Jan 3
- [ ] S02-01: As a gladiator, I want actor recognition
  - Acceptance/TDD: stable IDs; role, rank and relationship distinct.
- [ ] S02-02: As a gladiator, I want combat stats
  - Acceptance/TDD: define power semantics; reject invalid health, attack, defense and power.
- [ ] S02-03: As a gladiator, I want social context
  - Acceptance/TDD: test conversation capability, activity, alive and distance independently.
- [ ] S02-04: As model developer, I want encoded features
  - Acceptance/TDD: version schema; unknown differs from zero; confidence and age validated.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## S03 — Jan 11–24 2027
- [ ] S03-01: As a gladiator, I want conversation eligibility
  - Acceptance/TDD: reject dead or out-of-range participants.
- [ ] S03-02: As a gladiator, I want combat policy
  - Acceptance/TDD: test Talk/Attack/Defend/Retreat/Ignore baseline.
- [ ] S03-03: As a gladiator, I want target selection
  - Acceptance/TDD: deterministic ties and eligible targets only.
- [ ] S03-04: As developer, I want legal action validation
  - Acceptance/TDD: safe fallback when none eligible; demo two actors.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-03] Synthetic training data generation — [#4](https://github.com/medevacvii/ColosseumV3/issues/4)

Parent tracking: #1
Dependency: #3 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S04 — Jan 25–Feb 7 2027
- [ ] S04-01: As developer, I want synthetic actors
  - Acceptance/TDD: valid ranges and deterministic seeds.
- [ ] S04-02: As developer, I want diverse encounters
  - Acceptance/TDD: include unknown features, conflicting evidence and varied roles.
- [ ] S04-03: As model developer, I want labels
  - Acceptance/TDD: check legal actions, document policy and class distribution.
- [ ] S04-04: As researcher, I want independent splits
  - Acceptance/TDD: grouped encounter/archetype separation; manifest and repeatable generation.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-04] Neural network training and evaluation — [#5](https://github.com/medevacvii/ColosseumV3/issues/5)

Parent tracking: #1
Dependency: #4 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S05 — Feb 8–21 2027
- [ ] S05-01: As developer, I want a small MLP
  - Acceptance/TDD: test input/output tensor shapes.
- [ ] S05-02: As learner, I want activation/loss rationale
  - Acceptance/TDD: finite loss and documented ReLU/classification choice.
- [ ] S05-03: As developer, I want reproducible training
  - Acceptance/TDD: controlled learning test and checkpoint reload.
- [ ] S05-04: As gladiator, I want action scores
  - Acceptance/TDD: test masking and held-out predictions.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## S06 — Feb 22–Mar 7 2027
- [ ] S06-01: As researcher, I want metrics
  - Acceptance/TDD: confusion matrix, macro F1 and per-action recall verified against examples.
- [ ] S06-02: As developer, I want learning curves
  - Acceptance/TDD: diagnose under/overfitting.
- [ ] S06-03: As developer, I want tuning
  - Acceptance/TDD: validation only; preprocessing fit on training only; grouped separation.
- [ ] S06-04: As product owner, I want deployment gate
  - Acceptance/TDD: predeclare thresholds, untouched test evaluation, baseline comparison and explicit approval; imitation is not superiority.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-05] ONNX, C++ and Blueprint integration — [#6](https://github.com/medevacvii/ColosseumV3/issues/6)

Parent tracking: #1
Dependency: #5 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S07 — Mar 8–21 2027
- [ ] S07-01: As developer, I want ONNX export
  - Acceptance/TDD: validate opset and versioned schema.
- [ ] S07-02: As developer, I want inference parity
  - Acceptance/TDD: compare PyTorch/ONNX within agreed tolerance.
- [ ] S07-03: As Unreal developer, I want C++ adapter
  - Acceptance/TDD: verify supported NNE runtime and reject schema mismatch.
- [ ] S07-04: As player, I want fallback
  - Acceptance/TDD: invalid model and input fail safely.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## S08 — Mar 22–Apr 4 2027
- [ ] S08-01: As Blueprint developer, I want facade
  - Acceptance/TDD: request and receive decision.
- [ ] S08-02: As gladiator, I want context components
  - Acceptance/TDD: missing components fail safely.
- [ ] S08-03: As developer, I want executor
  - Acceptance/TDD: invalid recommendations cannot bypass C++ validation.
- [ ] S08-04: As tester, I want isolated demo
  - Acceptance/TDD: two actors and diagnostics; unrelated assets unchanged.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-06] Actor perception and persistent memory — [#7](https://github.com/medevacvii/ColosseumV3/issues/7)

Parent tracking: #1
Dependency: #6 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S09 — Apr 5–18 2027
- [ ] S09-01: As gladiator, I want observations
  - Acceptance/TDD: no new visual data when unobserved.
- [ ] S09-02: As gladiator, I want estimated stats
  - Acceptance/TDD: contradictory evidence revises beliefs.
- [ ] S09-03: As gladiator, I want confidence and freshness
  - Acceptance/TDD: validate per-stat age and unknown masks.
- [ ] S09-04: As model developer, I want hybrid policy
  - Acceptance/TDD: train on perceived features; hidden-stat changes alone cannot change deterministic inference.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## S10 — Apr 19–May 2 2027
- [ ] S10-01: As gladiator, I want recognition
  - Acceptance/TDD: stable actor ID retrieval.
- [ ] S10-02: As gladiator, I want relationship memory
  - Acceptance/TDD: independent trust/fear/familiarity per observer.
- [ ] S10-03: As gladiator, I want aging
  - Acceptance/TDD: stale estimates lose confidence.
- [ ] S10-04: As player, I want save/load
  - Acceptance/TDD: restart restores supported fields; weights unchanged by encounters.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-07] Multi-actor interaction and social intelligence — [#8](https://github.com/medevacvii/ColosseumV3/issues/8)

Parent tracking: #1
Dependency: #7 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S11 — May 3–16 2027
- [ ] S11-01: As gladiator, I want multiple candidates
  - Acceptance/TDD: exclude ineligible partners.
- [ ] S11-02: As gladiator, I want status-sensitive behavior
  - Acceptance/TDD: test role and relationship independently.
- [ ] S11-03: As player, I want personality differences
  - Acceptance/TDD: approved courage/aggression/risk scenarios yield differentiated behavior.
- [ ] S11-04: As gladiator, I want conversation intent
  - Acceptance/TDD: greet/warn/negotiate/cooperate separate from dialogue generation.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.

## [AI-08] System validation and MVP release — [#9](https://github.com/medevacvii/ColosseumV3/issues/9)

Parent tracking: #1
Dependency: #8 must be accepted before this group starts. Status: not started; individual sprint scope requires approval. Sundays off; holiday pause Dec 21–Jan 3.

## S12 — May 17–30 2027
- [ ] S12-01: As developer, I want end-to-end regression
  - Acceptance/TDD: Python/Unreal tests, save/load and fallback pass.
- [ ] S12-02: As player, I want responsiveness
  - Acceptance/TDD: measure agreed hardware-specific budget.
- [ ] S12-03: As teammate, I want maintainable docs
  - Acceptance/TDD: architecture, schema, setup and actual patterns reviewed.
- [ ] S12-04: As product owner, I want final demo
  - Acceptance/TDD: VR/non-VR supported smoke tests and observation-to-memory demonstration; explicit acceptance.
- [ ] Record observed RED/GREEN/REFACTOR where applicable; review and demonstration accepted.

## Group acceptance
- [ ] All stories and sprint reviews accepted explicitly by product owner.
- [ ] Actual tests, documentation, AI log and commit/PR evidence recorded.
Use Refs #N until entire group accepted. No unrelated Unreal assets.
