# Gladiator AI Model — Scrum Development Schedule

Status: Proposed roadmap; all sprints pending. Created 2026-11-23.
Branch: AI-model (from albert). Target: 2026-11-30 through 2027-05-30.
12 two-week active sprints; pause December 21–January 3. Dates are estimates, not commitments. Sundays are reserved for rest; finish reviews by Saturday.

## Product goal
Create context-aware gladiators that recognize actors, decide whether and with whom to converse, and choose actions using own exact statistics and permitted observations of others. Preserve individual memories without retraining model weights on each encounter. Keep C++ and Blueprint loosely coupled. Start with an exact-information baseline, then deploy a hybrid perception model.

## Backlog and sprint acceptance

| ID | Dates | Sprint goal and tasks | Acceptance / visible demo |
|---|---|---|---|
| S01 | Nov 30–Dec 13 | Python environment, pinned dependencies, pytest, Git workflow, reproducible seeds, architecture decision records | Clean checkout can run tests; demonstrate red-green-refactor on one function |
| S02 | Dec 14–Jan 10 (holiday pause) | Actor identity, role/rank, relationship, conversation capability, health, power (define semantics), attack, defense, distance, alive/activity; own vs other fields; known masks, confidence and age; versioned schema | Test validation, normalization, unknown vs zero, actor ID stability, self vs other isolation; print feature vector |
| S03 | Jan 11–24 | Deterministic omniscient baseline; legal action masks; Talk, Attack, Defend, Retreat, Ignore; select one target among eligible actors; debug-only exact opponent provider | Demo a single encounter; dead/out-of-range actors cannot be conversed with; deterministic ties; no legal action yields safe fallback |
| S04 | Jan 25–Feb 7 | Seeded synthetic encounter generator; policy labels; balanced scenarios, conflicting evidence, unknown features; grouped train/validation/test splits by encounter/actor archetype and scenarios | Dataset manifest, schema and distribution report; no overlap across splits; regenerate with same seed; label checks |
| S05 | Feb 8–21 | Small PyTorch MLP; categorical encoding, normalization, action scores; compare rule baseline; record experiments | Training and inference run on CPU; tests for tensor shape, finite outputs, masking, checkpoint load; demo predictions on held-out examples |
| S06 | Feb 22–Mar 7 | Evaluate confusion matrix, macro F1, per-action recall, learning curves; regularization/early stopping; assess overfitting, class imbalance, leakage, unseen archetypes; freeze acceptance thresholds before final test | Report baseline comparison and errors; held-out evaluation once after tuning; document that imitation of synthetic labels is not proof of superior strategy; approve deployment gate |
| S07 | Mar 8–21 | Export ONNX; pin feature schema and opset; verify supported Unreal NNE runtime; C++ inference adapter and fallback; model version/hash | Python/ONNX numerical parity within documented tolerance; unsupported model and malformed inputs safely fall back; demo C++ output |
| S08 | Mar 22–Apr 4 | Blueprint-facing facade, C++ action executor, test map isolated from team assets, debug overlay, multi-target score contract | Demo one gladiator interacting with another actor; verify C++ revalidates permissions and distance; no edits to unrelated team maps |
| S09 | Apr 5–18 | Perception events, line of sight, belief estimates, per-stat confidence and freshness; separate ground truth from observed state; train/evaluate hybrid model on realistic inputs | Hidden-stat isolation and leakage tests; unknown actor and contradictory observations demo; changed hidden stats alone do not change input or decision; compare exact and hybrid modes |
| S10 | Apr 19–May 2 | Per-gladiator persistent actor memory, stable IDs, trust/fear/familiarity, conversation history, aging and save/load versioning | Two gladiators remember same actor differently; restart restores memory; stale stats lose confidence; no model retraining required |
| S11 | May 3–16 | Multiple actors, rank/status-sensitive interactions, personality and risk tolerance; target scoring, talk intent (greet/warn/negotiate); scripted dialogue adapter; optional deception scenarios | Demo ally, enemy, neutral and higher-rank actor; choose eligible conversation partner; no forced interaction; separate dialogue content from decision policy |
| S12 | May 17–30 | End-to-end regression, performance profiling, save/load migration, invalid model fallback, VR/non-VR smoke tests, documentation and demo | All required tests pass; agreed frame/inference budget measured on target hardware; demo unknown opponent → observation → relationship → saved memory; review-ready PR |

## Architecture and guardrails
World stats → permitted perception → beliefs + memory → context builder → action scoring → legal action validation → Blueprint execution. Exact opponent data is for baseline/debug/evaluation only after hybrid rollout. Game engine may use actual stats to resolve outcomes, not secretly feed them to character inference. Use Strategy for knowledge providers, Observer for observation events, Adapter for ONNX/NNE, Repository for memories, Facade for Blueprint and Command for actions. These are proposed patterns, not a claim of existing implementations.

Use stable actor identifiers for memory lookup, not arbitrary numeric identity as a model feature. Separate role, social rank, relationship and current condition. Unknown is distinct from zero. Define power (e.g., current stamina versus inherent strength) before generating data. Start with per-candidate scoring and explicit target eligibility; no need to retrain when an unfamiliar actor appears. Dialogue generation/LLM integration and online weight updates are outside initial MVP.

## Scrum cadence
- Sprint planning: agree goal, estimate capacity, select small stories with testable acceptance criteria; no automatic assignment or scope expansion.
- Daily workday check-in: completed, next action, blocker (15 minutes maximum).
- Mid-sprint: review blockers, model metrics and integration risks.
- Sprint review: live demonstration and evidence; product owner accepts or returns incomplete work to backlog.
- Retrospective: one improvement action for next sprint. Replan remaining dates based on capacity.
- Roles: product owner prioritizes and accepts; developers implement/test; Scrum Master facilitates and removes blockers. Names to be agreed.

## Definition of done (every story)
- [ ] Acceptance criteria and test cases documented before implementation.
- [ ] Red → green → refactor evidence for deterministic code; ML experiments have reproducible seeds, dataset versions and evaluation checks.
- [ ] Relevant Python unit/integration tests and Unreal automation tests pass.
- [ ] No leakage from hidden actor state or holdout data; no unexpected access to private stats.
- [ ] Feature/model/schema changes versioned; dependencies and reproducibility documented.
- [ ] Small reviewable Git commit and PR; no unrelated Blueprint, .uasset or .umap edits; coordinate LFS locks if needed.
- [ ] Visible demo and test evidence attached; reviewer/product owner acceptance recorded.

## Risks and milestones
- M1 S03: playable deterministic decision baseline.
- M2 S06: evaluated model; do not deploy if quality gate fails.
- M3 S08: Unreal inference and Blueprint demonstration.
- M4 S10: perception and persistent individual memory.
- M5 S12: integrated MVP, review-ready.
- Synthetic labels may merely reproduce rules; compare against baseline and independent scenario tests.
- Overfitting: grouped splits, validation tuning, untouched test set, learning curves, regularization.
- Runtime compatibility: verify NNE runtime and ONNX operators early; deterministic fallback.
- Scope/capacity: protect team assets, use isolated test map and prioritize core features over advanced dialogue.

## Sprint tracking template
Sprint ID / goal / capacity / story ID / priority / estimate / owner / acceptance criteria / status (Backlog, Ready, In Progress, Review, Done, Blocked) / evidence / blockers / review decision / retrospective action.

## First story (S01-01)
As a developer, I want a repeatable Python test environment so that every teammate can verify changes. Acceptance: documented setup, clean environment, one failing test first, implementation makes it pass, full suite passes, and a short console demonstration. No Unreal assets modified.
