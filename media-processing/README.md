
## Media Processing Application

This application processes the media content uploaded by the users. 
- The [Convert Audio](#convert-audio-function) function converts the format of a media file. 
- The [Get Media Meta](#get-media-meta-function) function extracts the meta information from a media file.

### Prerequisite

Both functions access [OSS storage](https://www.alibabacloud.com/product/object-storage-service) (an Alibaba-cloud equivalent of S3). You have to create an OSS bucket and upload some videos to it. (`video-processing/data/video.mp4` is an example.)


### Convert Audio Function

To deploy the function,

```
cd faas-scheduling-benchmark/video-processing
fun deploy -y convert-audio
```

To invoke the function,

```
# Replace all bracketed placeholders "<xxx>" with your choice.
# - The <fc-endpoint> can be found in the result printed out by `fun deploy`.
# - The "object_url" is where you have uploaded the video file. 
curl -s -i http://<fc-endpoint>/2016-08-15/proxy/video-processing/convert-audio/ \
        -X POST \
        -d "{ \"object_url\": \"https://<bucket-name>.oss-<region>.aliyuncs.com/<path-to-media-file>\", 
              \"output_bucket\": \"<bucket-name>\", 
              \"output_prefix\": \"<path-prefix>\", 
              \"dst_type\": \"wav\", 
              \"params\": \"-ac 1 -ar 4000\" 
            }" 
```

The function converts an audio file into the WAV format and writes it to `<bucket-name>/<path-prefix>/`. 

### Get Media Meta Function

To deploy the function, 

```
cd faas-scheduling-benchmark/video-processing
fun deploy -y get-media-meta 
```

To invoke the function,

```
curl -s -i http://<fc-endpoint>/2016-08-15/proxy/video-processing/get-media-meta/ \
        -X POST \
        -d "{ \"object_url\": \"https://<bucket-name>.oss-<region>.aliyuncs.com/<path-to-media-file>\" }"
