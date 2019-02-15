# kocchiwork
A tool for editting remote file locally.

This application copy the remote file passed by command line to local directory and opens it, after finished editting the file, it will be copyed back to the original location.

## Special extention
If a file extention is ".kocchi", the local file will be renamed without this extention, so default application associated with extention after renamed could be found by the system.

## Command line options
```
/P : Specify program used to open the local file
/N : Not saved into history
/L : Specify language
```