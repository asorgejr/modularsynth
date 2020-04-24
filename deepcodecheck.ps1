# runs a deepcode analysis on the project sources.
$here = $PSScriptRoot
$lib = "${here}/lib"
& pyenv38.ps1
& deepcode analyze `
--path ("${here}/app") `
--path ("${here}/src") `
--path ("${lib}/libmodularsynth/include/modularsynth") `
--path ("${lib}/libmodularsynth/src/modularsynth") `
--path ("${lib}/nodesystem/include/nodesystem") `
--path ("${lib}/nodesystem/src/nodesystem") `
--with-linters `
--log-file="${here}/.log/.deepcode.log" `
-txt `
--severity warning