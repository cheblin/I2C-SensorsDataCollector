 $output_dir = $PSScriptRoot + "\gen"
 Start-Process -FilePath java.exe -ArgumentList ( "-jar", "D:\AdHoc\Agent\bin\AdHocAgent.jar", ($PSScriptRoot + "/src/org/company/PowerProfiler.java!") )  -WorkingDirectory $output_dir -Wait
 Copy-Item  -Path ($output_dir + "\InC\Device\*") -Include("*AdHoc.c", "*AdHoc.h" , "*config_.h", "*Device.c", "*Device.h", "*Utils_.h" ) -Destination $PSScriptRoot + "..\STM32F103\Main\adhoc" -Force