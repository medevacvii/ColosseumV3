# GitHub issue and sprint index

Approved setup; actual GitHub issues verified open. Governance [#1](https://github.com/medevacvii/ColosseumV3/issues/1).

| Issue | Scope | Sprint dates | Dependency |
|---|---|---|---|
| [#2](https://github.com/medevacvii/ColosseumV3/issues/2) | [AI-01] Development environment and TDD foundation | S01 — Nov 30–Dec 13 2026 | None |
| [#3](https://github.com/medevacvii/ColosseumV3/issues/3) | [AI-02] Actor features and rule-based intelligence | S02 — Dec 14 2026–Jan 10 2027; pause Dec 21–Jan 3; S03 — Jan 11–24 2027 | #2 |
| [#4](https://github.com/medevacvii/ColosseumV3/issues/4) | [AI-03] Synthetic training data generation | S04 — Jan 25–Feb 7 2027 | #3 |
| [#5](https://github.com/medevacvii/ColosseumV3/issues/5) | [AI-04] Neural network training and evaluation | S05 — Feb 8–21 2027; S06 — Feb 22–Mar 7 2027 | #4 |
| [#6](https://github.com/medevacvii/ColosseumV3/issues/6) | [AI-05] ONNX, C++ and Blueprint integration | S07 — Mar 8–21 2027; S08 — Mar 22–Apr 4 2027 | #5 |
| [#7](https://github.com/medevacvii/ColosseumV3/issues/7) | [AI-06] Actor perception and persistent memory | S09 — Apr 5–18 2027; S10 — Apr 19–May 2 2027 | #6 |
| [#8](https://github.com/medevacvii/ColosseumV3/issues/8) | [AI-07] Multi-actor interaction and social intelligence | S11 — May 3–16 2027 | #7 |
| [#9](https://github.com/medevacvii/ColosseumV3/issues/9) | [AI-08] System validation and MVP release | S12 — May 17–30 2027 | #8 |

Before each task and commit, agents search open AND closed issues, confirm scope/state/criteria and reuse matches. Never ask the user to look up numbers. No match: stop and flag; create only with authorization. Use Refs #N for partial or review-pending work. Closes #N only after every story and full issue acceptance, with closure intended; default-branch merge may trigger closure. Include references in API messages and PR descriptions; verify actual SHA/message/issue links after commit. For governance docs use Refs #1. See root AGENTS.md and .gitmessage.txt. No native Projects board is implied. Update progress only from actual evidence. Sundays off; pause Dec 21–Jan 3.
