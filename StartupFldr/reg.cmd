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

say ""
say "Try to Register class"
say ""

do until SysRegisterObjectClass("WPStartup2", WPStrtup)
   beep(1890,100)
   call SysSleep 1
   end

say ""
say 'WPStartup2 class registered'

beep(3000,100)
beep(2832,100)
beep(3000,100)

call SysCreateObject "WPStartup2", "Neuer Startupordner", "<WP_DESKTOP>"

exit
