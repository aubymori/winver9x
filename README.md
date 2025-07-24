<center>
    <img src="screenshot.png" alt="winver9x with Windows 98 SE style">
</center>

# winver9x
winver9x is a Win32 recreation of the `WINVER.EXE` applet from Windows 9x, the original version of which is
a Win16 application and will not run on modern systems.

## Configuration
winver9x can display different brandings. To do so, create the registry value
`HKEY_CURRENT_USER\SOFTWARE\winver9x\Mode` as a DWORD, and set it to one of
the following values:

- 0 - Windows 95, default
- 1 - Windows 98
- 2 - Windows 98 SE (1999 copyright date)