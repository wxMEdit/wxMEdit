#!/bin/sh

export LC_ALL=C

gawk -f changelog_rpm.awk ../../ChangeLog > changelog
cat wxmedit.spec._ changelog > wxmedit.spec
cat wxmedit-static.spec._ changelog > wxmedit-static.spec
