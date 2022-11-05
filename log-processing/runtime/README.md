
## Rust-Shim for Aliyun FC (Function Compute)

A scaffold for porting Rust functions to FC.


### Code Structure

- The `bootstrap` is a package that handles the FC protocol and relays the input and output of your function. 
- You have to wrap your function in a Rust package named `handler`, which exposes a function with the signature `fn handle(String) -> String` (UTF-8 encoded).
- Your function package is supposed to be put in a directory called `function` and side by side with the `bootstrap` for building.

```
├── function
└── bootstrap
```

