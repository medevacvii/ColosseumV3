# Albert's proposal: organizing our Unreal Engine knowledge

Author: Albert Leung  
Date: October 30, 2026  
Status: Personal proposal for discussion; not a mandatory team standard.

## Purpose and scope

Use ColosseumV3 to preserve reusable Unreal Engine knowledge: setup instructions, working examples, troubleshooting notes, screenshots, and explanations.

The team agrees on the big picture, but individual members may implement the details differently. This document records Albert's recommended approach. Its folder names, branch names, and workflow are suggestions, not a claim that the team has approved every detail.

Saving this proposal on the `albert` branch keeps it available for review without establishing it as policy on `main`.

## Recommendation: folders for categories, branches for work

Use folders to organize permanent subjects. Use branches to develop and review changes.

A branch is a separate version of the repository, not a folder. If permanent subjects exist only on separate branches, readers must switch branches to find them. Shared instructions can drift apart, links become harder to maintain, and knowledge is harder to browse together.

| Repository element | Recommended purpose |
|---|---|
| `main` | Reviewed knowledge available together in one place |
| Subject folders | Permanent knowledge categories |
| Working branches | Draft guides, experiments, and proposed changes |
| `albert` | Albert's proposals and personal work pending any team review |

## Suggested folder structure

These paths are illustrative and are not created by this document.

| Suggested path | Contents |
|---|---|
| `Knowledge/README.md` | A short index linking to the guides |
| `Knowledge/VisualStudio/` | Unreal Engine and Visual Studio setup and fixes |
| `Knowledge/JetBrainsRider/` | Unreal Engine and Rider setup and fixes |
| `Knowledge/Blueprints/` | Blueprint concepts and small examples |
| `Knowledge/CPP/` | C++ concepts and C++/Blueprint interaction |
| `Knowledge/Git/` | Branches, pull requests, Git LFS, locking, and collaboration |
| `Knowledge/Troubleshooting/` | Cross-cutting problems and proven solutions |
| `IntegrationFixes/` | Optional, isolated reference projects and repaired asset examples |

Keep a subject-specific fix with its subject where practical. For example, a Rider setup error can live under `Knowledge/JetBrainsRider/`; the troubleshooting index can link to it rather than duplicate it.

Keep screenshots near the guide that uses them, for example:

```text
Knowledge/JetBrainsRider/
  Getting-Started.md
  images/
    rider-unreal-settings.png
```

## Current branches and suggested uses

Branch inventory checked on October 30, 2026. These four branches currently exist in ColosseumV3. The purposes below are recommendations; branch names do not establish team policy or prove that setup work is complete.

| Current branch | Suggested role | Examples of suitable work | Suggested destination when ready |
|---|---|---|---|
| [`main`](https://github.com/medevacvii/ColosseumV3/tree/main) | Shared baseline and reviewed knowledge | Approved guides, a knowledge index, and agreed reference examples | Receive reviewed changes through pull requests |
| [`albert`](https://github.com/medevacvii/ColosseumV3/tree/albert) | Albert's personal proposals and implementation approach | This organization proposal, draft notes, and alternatives for discussion | Keep personal material here; propose selected changes to `main` only when appropriate |
| [`integration/unreal-visual-studio`](https://github.com/medevacvii/ColosseumV3/tree/integration/unreal-visual-studio) | Develop and check Unreal Engine + Visual Studio integration knowledge | Installation steps, toolchain configuration, build troubleshooting, screenshots, and isolated examples | Pull request to `main` for reviewed material, preferably under `Knowledge/VisualStudio/` |
| [`integration/unreal-jetbrains-rider`](https://github.com/medevacvii/ColosseumV3/tree/integration/unreal-jetbrains-rider) | Develop and check Unreal Engine + JetBrains Rider integration knowledge | Rider setup, Unreal integration, debugging notes, screenshots, and isolated examples | Pull request to `main` for reviewed material, preferably under `Knowledge/JetBrainsRider/` |

The integration branches and `albert` were created from `main`. Their existence does not mean integration has already been configured or tested. This inventory records branch names, not a review of all branch contents.

### Choosing a branch

1. Writing a Visual Studio integration guide? Use `integration/unreal-visual-studio`.
2. Writing a Rider integration guide? Use `integration/unreal-jetbrains-rider`.
3. Recording Albert's preferred approach or a proposal whose details are not agreed? Use `albert`.
4. Looking for the shared reviewed version? Start on `main`.

A branch contains a version of the whole repository. Put a guide in its subject folder even while working on a subject branch. Merging that guide into `main` can then preserve its category.

Do not merge the entire `albert` branch just to publish one guide if it also contains unrelated personal proposals. Use a focused topic branch or selected commits and inspect the pull request's changed-file list.

For later work, short-lived topic branches can make each pull request easier to review. The team can retain the current branches if that better fits its workflow. Additional personal or subject branches are optional and are not created by this proposal.

Update this inventory when branches are added, renamed, or removed. Members can agree on the shared outcome while choosing different detailed implementations.

## Small hands-on example

Goal: write a Rider setup guide.

1. Start with the latest agreed baseline and switch to a suitable working branch.
2. Create `Knowledge/JetBrainsRider/Getting-Started.md`.
3. Write one small task at a time, including what the reader should see after each task.
4. Store screenshots in the nearby `images/` folder and use relative links.
5. Record the Unreal Engine, Rider, operating system, and relevant toolchain versions actually tested.
6. Commit and push the guide to the working branch.
7. Open a pull request to `main` when ready for team review.
8. Review the changed-file list before merging, especially any Unreal assets.
9. After approval and merge, readers can find the finished guide alongside the other subjects on `main`.

This proposal itself remains on `albert` unless the team explicitly decides to merge it.

## Suggested guide format

Each guide should answer these questions:

- What will I achieve?
- What must already be installed or available?
- Which versions were tested?
- What are the numbered steps?
- What visible result confirms each important step worked?
- What common errors might appear, and how do I recover?
- Which example files or related guides should I open?

Mark untested suggestions clearly. Do not describe a possible fix as proven until someone has reproduced the result.

## Preserving repaired Unreal assets

If a fix needs `.uasset` or `.umap` files, keep the reference example isolated from the team's active project assets. A possible location is:

```text
IntegrationFixes/VisualStudio-Unreal/TestProject/
```

A folder alone does not make an Unreal asset example portable or runnable. Copied assets can depend on other assets, plugins, project settings, and engine versions. When preparing an example:

1. Prefer a minimal independent Unreal project.
2. Use Unreal's asset migration workflow where appropriate to include required dependencies.
3. Document required plugins, settings, engine version, and steps to reproduce the fix.
4. Verify the example opens independently before calling it a working example.
5. Use the repository's agreed Git LFS rules for binary assets.
6. Exclude generated caches and build outputs according to the project's ignore rules.
7. Explain how a teammate can inspect or reproduce the fix deliberately.

Do not overwrite active team maps or Blueprints merely to demonstrate an integration fix. A reference example should let teammates compare approaches and choose what to apply.

## Allowing different implementations

Team members may prefer different IDEs, folder layouts, or setup methods. Record alternatives with their prerequisites, tested versions, and results rather than assuming one method fits every machine.

If the team chooses permanent subject branches instead, give each branch a clear README and maintain a central index with explicit branch links. This is workable, but requires extra effort to keep shared material consistent.

The proposed shared outcome is accessible, reproducible knowledge. The team can agree on that outcome while deciding the detailed structure separately.

## Adoption

This commit documents a recommendation only. It does not create the proposed directory structure, change Unreal settings, move assets, or merge anything into `main`.

If the team later adopts any part of the proposal, agree on the specific convention in a separate reviewed change.
