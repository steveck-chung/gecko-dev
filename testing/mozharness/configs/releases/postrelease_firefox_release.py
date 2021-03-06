config = {
    "log_name": "bump_release",
    "version_files": [
        {"file": "browser/config/version.txt"},
        {"file": "browser/config/version_display.txt"},
        {"file": "config/milestone.txt"},
    ],
    "repo": {
        "repo": "https://hg.mozilla.org/releases/mozilla-release",
        "branch": "default",
        "dest": "mozilla-release",
        "vcs": "hg",
    },
    "vcs_share_base": "/builds/hg-shared",
    "push_dest": "ssh://hg.mozilla.org/releases/mozilla-release",
    "ignore_no_changes": True,
    "ssh_user": "ffxbld",
    "ssh_key": "~/.ssh/ffxbld_rsa",
}
