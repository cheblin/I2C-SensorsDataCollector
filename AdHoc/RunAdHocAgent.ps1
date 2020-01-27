 $output_dir = $PSScriptRoot + "\gen"
 Start-Process -FilePath java.exe -ArgumentList ( "-jar", "D:\AdHoc\Agent\bin\AdHocAgent.jar", ($PSScriptRoot + "/src/org/company/PowerProfiler.java!") )  -WorkingDirectory $output_dir -Wait
<<<<<<< HEAD
 Copy-Item  -Path ($output_dir + "\InC\Device\*") -Include("*AdHoc.c", "*AdHoc.h" , "*config_.h", "*Device.c", "*Device.h", "*Utils_.h" ) -Destination $PSScriptRoot + "..\STM32F103\Main\adhoc" -Force
=======
 Copy-Item  -Path ($output_dir + "\InC\Device\*") -Include("*AdHoc.c", "*AdHoc.h" , "*config_.h", "*Device.c", "*Device.h", "*Utils_.h" ) -Destination $PSScriptRoot + "..\STM32F103\Main\adhoc" -Force
>>>>>>> d5b35fee8b499fc7aa68f98d3685a4798e2471f7
