diskgen /u DRIVE wpsac.pkg d:\work\c\wpsac\install_deu\dest d:\work\c\wpsac\install_deu\source
if not ERRORLEVEL 0 goto ErrExit
copy EPFISINC.PKG dest
copy INSTALL.EXE  dest
copy INSTALL.IN_  dest
copy WPSAC.GEN    dest\WPSAC.PKG
copy EPFISINC.GEN dest\EPFISINC.PKG

:ErrExit
