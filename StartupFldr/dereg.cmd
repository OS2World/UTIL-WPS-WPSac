/* Register class WPMag */
echo off

Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

call SysDeregisterObjectClass "WPStartup2";
RetCode = result;

if RetCode then
  say 'Deregister successfully completed'
else
  say 'Could not deregister classes'

exit
