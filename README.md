# xAOD Decoration Example
This RootCore package example uses `EL::Worker::xaodStore` and `TObject::auxdecor<type>("name")` in conjunction to provide an example of:
- how to set and get decorations on objects (requires shallow copies of input)
- how to share data between algorithms, such as passing data along a chain of algorithms
- how to manage the `xaodStore` correctly

## Installing and Running

```
mkdir myExampleDir && cd myExampleDir
rcSetup Base,2.0.18
git clone git@github.com:ATLASConnect/xAOD_Decoration_Example.git
rc find_packages
rc compile
testRun
```

### Caveats

You might want to edit [util/testRun.cxx](util/testRun.cxx) to provide a path to a file to use.


#### Authors
- [Giordon Stark](https://github.com/kratsg)
