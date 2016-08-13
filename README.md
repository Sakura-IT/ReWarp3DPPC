ReWarp3DPPC
===========

This project is a Warp3D wrapper for use with ReWarp (a WarpOS reimplementation
for AmigaOS 4).

For more information about ReWarp see here:
https://github.com/Sakura-IT/ReWarp

Automated binary builds are available from Jenkins: https://sonnet.uguu.waw.pl/jenkins/job/ReWarp3DPPC

ReWarp3DPPC supports the following arguments (in the form of a file) in ENVARC:Warp3DPPC/

PatchDrawElements - Use Alain Thellier's patch for W3D_DrawElements
PatchDrawArray - Use Alain Thellier's patch for W3D_DrawArray
PatchAll - Use all of the above

Recommended is PatchAll. This has been tested on a Sam440 with a Radeon 9200 on AmigaOS 4.1FE.

