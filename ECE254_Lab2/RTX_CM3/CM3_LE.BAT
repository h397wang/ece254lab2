SET PATH=c:\Software\Keil\ARM\ARMCC\bin\;C:\Software\Python36\Scripts\;C:\Software\Python36\;C:\Software\Perl64\site\bin;C:\Software\Perl64\bin;C:\Software\Python35\Scripts\;C:\Software\Python35\;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;c:\Program Files\TortoiseSVN\bin;C:\Program Files\MATLAB\R2014a\runtime\win64;C:\Program Files\MATLAB\R2014a\bin;C:\Program Files (x86)\WinSCP;C:\Program Files (x86)\GtkSharp\2.12\bin;C:\Program Files (x86)\Google\Chrome\Application;C:\Software\Python36\Scripts\;C:\Software\Python36\;C:\Software\Perl64\site\bin;C:\Software\Perl64\bin;C:\Software\Python35\Scripts\;C:\Software\Python35\;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;c:\Program Files\TortoiseSVN\bin;C:\Program Files\MATLAB\R2014a\runtime\win64;C:\Program Files\MATLAB\R2014a\bin;C:\Program Files (x86)\WinSCP
SET CPU_TYPE=Cortex-M3
SET CPU_VENDOR=ARM
SET UV2_TARGET=CM3_LE
SET CPU_CLOCK=0x00B71B00
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_task.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_system.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_event.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_list.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_mailbox.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_semaphore.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_time.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_timer.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_mutex.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_robin.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_membox.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_task_ext.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\rt_membox_ext.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\cm3_le\svc_table._ia"
"c:\Software\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\cm3_le\hal_cm3.__i"
"c:\Software\Keil\ARM\ARMCC\bin\ArmAr" --Via "ArInp.Scr"
cmd.exe /C copy ArInp.Scr CM3_LE\BuildLib.scr
