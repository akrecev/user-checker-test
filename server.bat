@echo off
call npm init -y
call npm i node-addon-api
call npm i express
call npm i open
call npm i
call node-gyp configure
call node-gyp build
node ./index.js