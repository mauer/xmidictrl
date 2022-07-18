#!/bin/bash
echo "--------------------- Make Project Package --------------------"

echo "-- copy 'examples'"
cp -r resources/examples package/XMidiCtrl

echo "-- copy 'templates'"
cp -r resources/templates package/XMidiCtrl

echo "-- copy 'changelog'"
cp docs/changelog.md package/XMidiCtrl

echo "-- copy 'readme'"
cp docs/README.md package/XMidiCtrl

echo "-- copy 'license'"
cp LICENSE package/XMidiCtrl

