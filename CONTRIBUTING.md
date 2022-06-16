# Contributing

Thank you for your interest in contributing to Cronos Play Unreal SDK! Good places to start are this document and the materials linked in README. If you have any questions, feel free to ask on [GitHub Discussions](https://github.com/crypto-com/play-unreal-plugin/discussions/landing).


## Code of Conduct

All contributors are expected to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## Feature requests and bug reports

Feature requests and bug reports should be posted as [Github issues](issues/new).
In an issue, please describe what you did, what you expected, and what happened instead.

If you think that you have identified an issue with Cronos Play Unreal SDK that might compromise
its users' security, please do not open a public issue on GitHub. Instead,
we ask you to refer to [security policy](SECURITY.md).

## Working on issues
There are several ways to identify an area where you can contribute to Cronos Play Unreal SDK:

* You can reach out by sending a message in the developer community communication channel, either with a specific contribution in mind or in general by saying "I want to help!".
* Occasionally, some issues on Github may be labelled with `help wanted` or `good first issue` tags.

We use the variation of the "fork and pull" model where contributors push changes to their personal fork and create pull requests to bring those changes into the source repository.
Changes in pull requests should satisfy "Patch Requirements" described in [The Collective Code Construction Contract (C4)](https://rfc.zeromq.org/spec:42/C4/#23-patch-requirements). The code should follow [Gamemakin UE4/UE5 Style Guide](https://github.com/Allar/ue5-style-guide)
except for where it is not easily possible (e.g. in auto-generated C++ bindings). Many of the style rules are captured by [this Linter plugin](https://ue4-style-guide.readthedocs.io/en/latest/gettingstarted.html), so please make sure to run it before every commit.

Once you identified an issue to work on, this is the summary of your basic steps:

* Fork Cronos Play Unreal SDK's repository under your Github account.

* Clone your fork locally on your machine.

* Post a comment in the issue to say that you are working on it, so that other people do not work on the same issue.

* Create a local branch on your machine by `git checkout -b branch_name`.

* Commit your changes to your own fork -- see [C4 Patch Requirements](https://rfc.zeromq.org/spec:42/C4/#23-patch-requirements) for guidelines.

* Check you are working on the latest version on main in Cronos Play Unreal SDK's official repository. If not, please pull Cronos Play Unreal SDK's official repository's main (upstream) into your fork's main branch, and rebase your committed changes or replay your stashed changes in your branch over the latest changes in the upstream version.

* If your changes are of interest to other developers, please make corresponding changes in the official documentation and the changelog.

* Push your changes to your fork's branch and open the pull request to Cronos Play Unreal SDK's repository main branch.

* In the pull request, complete its checklist, add a clear description of the problem your changes solve, and add the following statement to confirm that your contribution is your own original work: "I hereby certify that my contribution is in accordance with the Developer Certificate of Origin (https://developercertificate.org/)."

* The reviewer will either accept and merge your pull request, or leave comments requesting changes via the Github PR interface (you should then make changes by pushing directly to your existing PR branch).

### Developer Certificate of Origin
All contributions to this project are subject to the terms of the Developer Certificate of Origin, available [here](https://developercertificate.org/) and reproduced below:

```
Developer Certificate of Origin
Version 1.1

Copyright (C) 2004, 2006 The Linux Foundation and its contributors.
1 Letterman Drive
Suite D4700
San Francisco, CA, 94129

Everyone is permitted to copy and distribute verbatim copies of this
license document, but changing it is not allowed.

Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I
    have the right to submit it under the open source license
    indicated in the file; or

(b) The contribution is based upon previous work that, to the best
    of my knowledge, is covered under an appropriate open source
    license and I have the right under that license to submit that
    work with modifications, whether created in whole or in part
    by me, under the same open source license (unless I am
    permitted to submit under a different license), as indicated
    in the file; or

(c) The contribution was provided directly to me by some other
    person who certified (a), (b) or (c) and I have not modified
    it.

(d) I understand and agree that this project and the contribution
    are public and that a record of the contribution (including all
    personal information I submit with it, including my sign-off) is
    maintained indefinitely and may be redistributed consistent with
    this project or the open source license(s) involved.
```    