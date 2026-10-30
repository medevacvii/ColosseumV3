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

## Commands: switch branches and synchronize with GitHub

Run these commands in a terminal inside your local **ColosseumV3** repository. They work in PowerShell and Git Bash. Use the numbered sections as separate recipes; do not run every example in sequence.

### 1. First-time setup or checking the repository

If you have not downloaded this repository yet, open a terminal in the parent folder where you want it stored:

```bash
git clone https://github.com/medevacvii/ColosseumV3.git
cd ColosseumV3
```

If you already have a local copy, open a terminal in that folder instead of cloning again.

Check the repository and working files:

```bash
git remote -v
git status
git branch --show-current
```

Expected result: `origin` refers to `medevacvii/ColosseumV3`, and Git shows the current branch. If the remote names or repository differ, resolve that before using the following commands.

Close Unreal Editor before switching branches that contain Unreal assets. Save and commit unfinished work, or stash it, before switching so edits do not accidentally follow you to another branch.

Optional temporary storage for unfinished tracked and untracked files:

```bash
git stash push -u -m "Unfinished work before switching branches"
```

Return to the original branch before restoring with `git stash pop`. Restoration can produce conflicts. Ignored files are not included by `-u`.

### 2. See local and remote branches

```bash
git fetch origin
git branch
git branch -r
git branch -vv
```

- `fetch` downloads remote branch information and commits; it does not switch branches or update your working files.
- `git branch` lists local branches. The asterisk marks your current branch.
- `git branch -r` lists locally recorded remote branches such as `origin/albert`.
- `git branch -vv` shows upstream tracking and any ahead/behind information relative to the last fetch.

### 3. A branch exists on GitHub: create its local tracking branch

After `git fetch origin`, run **one** matching command below for a branch you have not yet created locally:

| Branch wanted | First-time local creation and switch |
|---|---|
| Albert | `git switch --track origin/albert` |
| Visual Studio | `git switch --track origin/integration/unreal-visual-studio` |
| JetBrains Rider | `git switch --track origin/integration/unreal-jetbrains-rider` |
| Main, only if missing locally | `git switch --track origin/main` |

Expected result: Git creates the local branch, switches to it, and configures its upstream to the matching branch on `origin`. Cloning normally creates local `main` already.

If Git says the branch already exists locally, use section 4 instead. If it cannot find `origin/...`, check spelling and run `git branch -r`.

### 4. Switch between branches that already exist locally

Run the single command for the branch you want:

| Destination | Command |
|---|---|
| Albert | `git switch albert` |
| Visual Studio | `git switch integration/unreal-visual-studio` |
| JetBrains Rider | `git switch integration/unreal-jetbrains-rider` |
| Main | `git switch main` |
| Previous branch | `git switch -` |

Confirm the result:

```bash
git branch --show-current
git status
```

Switching changes the checked-out version of the repository. It does not merge branches or automatically download new GitHub changes.

### 5. Download updates for your current branch

Once its upstream is configured:

```bash
git pull --ff-only
```

Expected result: your current local branch receives upstream commits, or Git reports it is already up to date. This updates from that branch's upstream, not automatically from `main`.

If Git reports diverging histories, stop and inspect before choosing a merge or rebase:

```bash
git fetch origin
git status
git log --oneline --graph --decorate -15
```

Do not use a force push or hard reset as a routine synchronization fix.

### 6. A branch exists locally: create its matching GitHub branch

Example: you already have a local branch named `notes/blueprint-learning`, and want to publish it. This is an illustrative name, not an existing branch in the inventory above.

```bash
git switch notes/blueprint-learning
git push -u origin notes/blueprint-learning
```

Expected result: GitHub receives the branch and its committed history, and `-u` sets the upstream. Uncommitted edits are not uploaded.

For an existing local branch from the inventory, the equivalent publish commands are:

| Local branch | Publish and set upstream |
|---|---|
| `albert` | `git push -u origin albert` |
| `integration/unreal-visual-studio` | `git push -u origin integration/unreal-visual-studio` |
| `integration/unreal-jetbrains-rider` | `git push -u origin integration/unreal-jetbrains-rider` |

If the remote branch already exists, Git attempts a normal update. If it rejects the push because GitHub contains work you do not have, fetch and inspect; do not overwrite teammates' commits.

If both local and remote branches already exist and you only need to connect their tracking relationship, for example:

```bash
git fetch origin
git branch --set-upstream-to=origin/albert albert
```

This sets tracking only; it does not synchronize file contents or reconcile different histories.

### 7. Create a brand-new local branch and publish it

Example: begin a new Blueprint guide from the latest `main`. Start with a clean working tree:

```bash
git switch main
git pull --ff-only
git switch -c notes/blueprint-learning
git push -u origin notes/blueprint-learning
```

Expected result: the new branch exists locally and on GitHub, with upstream tracking configured. Choose a different name if that branch already exists. The starting point is `main`; personal drafts on `albert` are not automatically included.

### 8. Everyday example: update this proposal on Albert's branch

First switch to your local `albert` branch (create it using section 3 if needed), then download its latest updates:

```bash
git switch albert
git pull --ff-only
```

Edit and save `KNOWLEDGE_ORGANIZATION_PROPOSAL.md`. Then review and upload that specific document:

```bash
git diff -- KNOWLEDGE_ORGANIZATION_PROPOSAL.md
git add KNOWLEDGE_ORGANIZATION_PROPOSAL.md
git diff --cached
git commit -m "docs: update knowledge organization proposal"
git push
git status
```

Review all staged changes before committing; previously staged files can also be included in the commit. Expected result: the document is committed and pushed to `albert`, and Git reports the branch is up to date with `origin/albert`.

If `git push` reports that no upstream exists, run `git push -u origin albert`. Authentication and repository write permission are required for publishing.

### Quick reminder

| What you want | Command |
|---|---|
| Refresh knowledge of GitHub branches | `git fetch origin` |
| Bring remote Albert branch onto this computer for the first time | `git switch --track origin/albert` |
| Switch to an existing local Albert branch | `git switch albert` |
| Receive updates from the current branch's upstream | `git pull --ff-only` |
| Publish local Albert branch and set tracking | `git push -u origin albert` |
| Upload later commits after tracking is set | `git push` |

Synchronization is explicit: **fetch** learns about remote changes, **pull** brings upstream commits into the current branch, and **push** uploads committed work. Publishing a branch does not merge it into `main`.

## Cherry-pick: propose only selected commits for main

Cherry-pick copies the changes introduced by selected commits onto your current branch, usually creating new commit IDs. It does not merge the whole source branch. The original commits remain on the source branch.

Example: `albert` contains three commits:

| Example commit ID | Change | Include in the pull request? |
|---|---|---|
| `a1b2c3d` | Add a finished Rider guide | Yes |
| `b2c3d4e` | Add Albert's personal planning notes | No |
| `c3d4e5f` | Correct screenshots used by the Rider guide | Yes |

These IDs are placeholders. Replace them with actual commit IDs from your repository; do not paste the example IDs as real commands.

The suggested workflow is: inspect the commits, create a fresh branch from updated `main`, cherry-pick the selected commits in order, review the result, and submit a pull request to `main`.

### 1. Prepare and fetch the latest commits

Run inside your local ColosseumV3 repository:

```bash
git status
git fetch origin
```

Start with a clean working tree. Commit or stash unfinished work first using the earlier instructions. Close Unreal Editor before changing branches containing Unreal assets.

### 2. Find commits on Albert's remote branch that are not on main

```bash
git log --reverse --oneline origin/main..origin/albert
```

Expected result: commits reachable from `origin/albert` but not from `origin/main`, oldest first. Each line starts with a commit ID.

This compares the last-fetched GitHub versions. Unpushed local commits will not appear; to inspect those instead, use `origin/main..albert`.

For an integration branch, substitute its remote name, for example:

```bash
git log --reverse --oneline origin/main..origin/integration/unreal-jetbrains-rider
```

Inspect each candidate before choosing it. Replace `COMMIT_ID` with a real ID:

```bash
git show --stat COMMIT_ID
git show COMMIT_ID
```

Check the changed paths, actual changes, and dependencies. A later screenshot correction may depend on the earlier guide commit. If a selected commit depends on an excluded commit, split or revise the work rather than assuming cherry-pick will make it independent.

### 3. Create a fresh review branch from main

```bash
git switch main
git pull --ff-only
git switch -c review/selected-knowledge
```

Expected result: you are on `review/selected-knowledge`, starting from updated `main`. Choose a new unused branch name for another batch.

If the pull fails because local `main` has diverged, stop and inspect that difference before continuing.

### 4. Copy only the selected commits

Replace the placeholder IDs below with your actual choices:

```bash
git cherry-pick a1b2c3d
git cherry-pick c3d4e5f
```

Run the first command and check its result before running the next. Apply prerequisite commits first, usually oldest to newest. The personal-notes commit `b2c3d4e` is deliberately omitted.

You can also apply several selected commits in one command:

```bash
git cherry-pick a1b2c3d c3d4e5f
```

Use either the separate-command method or the combined method, not both. Explicit IDs help avoid accidentally including unwanted commits through a range.

After success, the review branch contains the selected changes. Local `main` and the source branch have not received those new commits.

### 5. If Git reports a conflict

A conflict means Git needs help combining changes. Do not start another cherry-pick while the current one is unresolved.

```bash
git status
```

For text files:

1. Open each conflicted file.
2. Review the competing changes and edit the file to the intended final content.
3. Remove conflict markers such as `<<<<<<<`, `=======`, and `>>>>>>>`.
4. Stage each resolved file using its actual path, for example `git add Knowledge/JetBrainsRider/Getting-Started.md`.
5. Continue:

```bash
git cherry-pick --continue
```

For binary Unreal files such as `.uasset` and `.umap`, text editing cannot resolve the conflict. Decide which asset version is required, coordinate with the asset owner, and verify the result in Unreal. If unsure, abort the cherry-pick and prepare an isolated reference example.

To abandon the currently active cherry-pick operation:

```bash
git cherry-pick --abort
```

With a combined multi-commit command, abort returns to before that sequence. With separate commands, aborting the second operation leaves a previously successful first cherry-pick in place.

If Git reports an empty cherry-pick, inspect the changes: they may already be present. Use `git cherry-pick --skip` only when you have confirmed that skipping that commit is correct.

### 6. Review exactly what the pull request will contain

```bash
git fetch origin
git log --oneline origin/main..HEAD
git diff --stat origin/main...HEAD
git diff --name-status origin/main...HEAD
git diff origin/main...HEAD
git status
```

Expected result: the selected guide and its required supporting files appear; excluded personal work does not.

Check that:

- Required screenshots and dependencies are included.
- Unrelated files and active team maps are absent unless deliberately included.
- Markdown links work and the guide describes tested behavior accurately.
- Any included Unreal example opens and works in the intended engine version.

The three-dot diff compares your branch against its common ancestor with `origin/main`, matching the usual pull-request view. If `main` advanced, review the GitHub PR checks and any conflicts too.

### 7. Publish the review branch and open a pull request

```bash
git push -u origin review/selected-knowledge
```

On GitHub:

1. Open the ColosseumV3 repository.
2. Choose **Pull requests → New pull request**.
3. Set **base** to `main`.
4. Set **compare** to `review/selected-knowledge`.
5. Review **Files changed**.
6. Explain which knowledge was selected and how it was checked.
7. Create the pull request and follow the team's review process.
8. Merge only after the required review and checks are satisfied.

Pushing the review branch does not merge anything into `main`.

### 8. After the pull request is merged

```bash
git switch main
git pull --ff-only
```

Expected result: your local `main` now contains the reviewed knowledge. Albert's source branch still contains its original commits and personal notes.

Cherry-picking normally gives copied commits new IDs. Therefore, `git log origin/main..origin/albert` can still list original commits whose changes have already reached `main`. Do not cherry-pick them again solely because they appear in that list. Check the merged PR and compare file changes. `git cherry -v origin/main origin/albert` can help identify patch-equivalent commits, but edited or squashed changes may not be recognized.

Do not later merge all of `albert` into `main` just to finish this workflow: that could also introduce the personal notes you intentionally excluded.

### Selecting files is different from selecting commits

Cherry-pick selects whole commits, including all changed files in each selected commit. It is not a file filter.

If one commit mixes a useful guide with unrelated personal notes or binary assets, prepare a clean commit on a separate branch containing only the intended changes, or split the commit carefully before proposing it. Review the diff and dependencies before publishing. Avoid rewriting a shared branch's history just to tidy a pull request.

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
