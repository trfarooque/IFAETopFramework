Installation
------------

```
rcSetup Base,2.3.50
git clone ssh://git@gitlab.cern.ch:7999/htx/IFAETopFramework.git
# if you want to get a tagged version
# cd IFAETopFramework; git checkout IFAETopFramework-XX-YY-ZZ; cd -
rc checkout IFAETopFramework/share/packages.txt
rc find_packages
rc clean
rc compile
```
