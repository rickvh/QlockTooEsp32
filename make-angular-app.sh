#!/bin/bash
echo "Building Angular application..."
cd qlocktoo-portal
npm run build:prod
echo "Sync changes to Android project"
npx cap sync
