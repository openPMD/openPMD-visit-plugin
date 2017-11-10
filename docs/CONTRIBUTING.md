# Contributing to the openPMD-VisIt-plugin

## How to contribute

### Forking the repository

In order to contribute, please fork the [main repository](https://github.com/openPMD/openPMD-visit-plugin):

- Click 'Fork' on the page of the main repository, in order to create a personal copy of this repository on your Github account. 

- Clone this copy to your local machine:
```
git clone git@github.com:<YourUserLogin>/openPMD-visit-plugin.git
```

### Implementing a new feature

- Switch to the development branch
```
git checkout dev
```

- Start a new branch from the development branch, in order to implement a new feature. (Choose a branch name that is representative of the feature that you are implementing)
```
git checkout -b <NewBranchName>
```

- Start coding. When your changes are ready, commit them.
```
git add <ChangedFiles>
git commit
```

### Testing your implementation

### Adding your change to the main repository

- Synchronize your branch with the main repository. (It may have changed while you where implementing local changes.) Resolve merging issues if any, and commit the corresponding changes.
```
git pull git@github.com:openPMD/openPMD-visit-plugin.git dev
```

- Push the changes to your personal copy on Github
```
git push -u origin <NewBranchName>
```

- Go on your Github account and create a pull request between your new feature branch and the dev branch of the main repository. Please add some text to the pull request to describe what feature you just implemented and why. Please also make sure that the automated tests (on Github) return no error.
