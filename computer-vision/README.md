
## Computer Vision Application

This application mimics a CV company that provides image processing service through API.
- The [Classify Image](#classify-image-function) function classifes an image into a certain category.
- The [Detect Object](#detect-object-function) function detects whether an image contains a certain object.

### Classify Image Function

To deploy a function,

```
cd faas-scheduling-benchmark/computer-vision/classify-image
fun deploy -y
```

To invoke a function,

```
# The <fc-endpoint> can be found in the result printed out by `fun deploy`.
curl -s -i http://<fc-endpoint>/2016-08-15/proxy/computer-vision/classify-image/ \
        -X POST \
        -d 'https://github.com/All-less/faas-scheduling-benchmark/raw/master/computer-vision/data/sample.jpeg'
```

### Detect Object Function

To deploy a function,

```
cd faas-scheduling-benchmark/computer-vision/detect-object
fun deploy -y
```

To invoke a function,

```
# The <fc-endpoint> can be found in the result printed out by `fun deploy`.
curl -s -X POST -i http://<fc-endpoint>/2016-08-15/proxy/computer-vision/detect-object/ 

# Currently the detect-object function does not accept custom images. It comes with a set 
# of images and every time the function is invoked, it will randomly pick one and run the 
# detection algorithm on that image.
```
