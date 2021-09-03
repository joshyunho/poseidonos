## poseidonos-cli array addspare

Add a device as spare to an array.

### Synopsis

Add a device as spare to an array.

Syntax:
	poseidonos-cli array addspare (--spare | -s) SpareDeviceName (--array-name | -a) ArrayName

Example: 
	poseidonos-cli array addspare --spare nvme5 --array-name Array0
          

```
poseidonos-cli array addspare [flags]
```

### Options

```
  -a, --array-name string   Name of the array to add a spare device
  -h, --help                help for addspare
  -s, --spare string        Name of the device to add to an array
```

### Options inherited from parent commands

```
      --debug         Print response for debug
      --fs string     Field separator for the output (default "|")
      --ip string     Set IPv4 address to PoseidonOS for this command (default "127.0.0.1")
      --json-req      Print request in JSON form
      --json-res      Print response in JSON form
      --port string   Set the port number to PoseidonOS for this command (default "18716")
      --unit          Display unit (B, KB, MB, ...) when displaying capacity
```

### SEE ALSO

* [poseidonos-cli array](poseidonos-cli_array.md)	 - Array command for PoseidonOS.
