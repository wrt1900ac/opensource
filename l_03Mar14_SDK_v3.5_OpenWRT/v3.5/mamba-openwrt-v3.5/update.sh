#!/bin/sh
./scripts/feeds update packages
./scripts/feeds update luci
./scripts/feeds update routing
./scripts/feeds update xwrt
./scripts/feeds install -a -p packages
./scripts/feeds install -a -p luci
./scripts/feeds install -a -p routing

