
## FaaS Scheduling Benchmark

This benchmark suite is designed for evaluating the scheduler in a Function-as-a-Service platform. It consists of functions from various application domains so as to simulate the mixed workload in production.

### Prerequisite

If you want to deploy and run the functions, you need to ...

1. ... have an [Alibaba Cloud](https://us.alibabacloud.com/) account because the functions are implemented for [Function Compute](https://www.alibabacloud.com/product/function-compute).

2. ... know how to use [`fun` CLI](https://github.com/alibaba/funcraft) tool. If not, you could check out this [Get Started](https://github.com/alibaba/funcraft/blob/master/docs/usage/getting_started.md) guide.


### Repo Structure

The benchmark suite has five applications, each with two functions. You could follow the README in each application directory to deploy and invoke the function.

<table>
<thead>
  <tr>
    <th>Application</th>
    <th>Function</th>
    <th>Programming Language</th>
    <th>Dependencies</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td rowspan="2">Smart Parking</td>
    <td>Query Vacancy</td>
    <td>JavaScript</td>
    <td>Redis</td>
  </tr>
  <tr>
    <td>Reserve Spot</td>
    <td>JavaScript</td>
    <td>Redis, Kafka</td>
  </tr>
  <tr>
    <td rowspan="2">Log Processing</td>
    <td>Anonymize Log</td>
    <td>Rust</td>
    <td>Kafka</td>
  </tr>
  <tr>
    <td>Filter Log</td>
    <td>Rust</td>
    <td>Kafka</td>
  </tr>
  <tr>
    <td rowspan="2">Computer Vision</td>
    <td>Detect Object</td>
    <td>Python</td>
    <td>TensorFlow</td>
  </tr>
  <tr>
    <td>Classify Image</td>
    <td>Python</td>
    <td>TensorFlow</td>
  </tr>
  <tr>
    <td rowspan="2"><a href="media-processing">Media Processing</a></td>
    <td>Get Media Meta</td>
    <td>Python</td>
    <td>OSS</td>
  </tr>
  <tr>
    <td>Convert Audio</td>
    <td>Python</td>
    <td>OSS</td>
  </tr>
  <tr>
    <td rowspan="2">Smart Manufacturing</td>
    <td>Ingest Data</td>
    <td>C++</td>
    <td>MySQL</td>
  </tr>
  <tr>
    <td>Detect Anomaly</td>
    <td>C++</td>
    <td>MySQL</td>
  </tr>
</tbody>
</table>

### Portability

The benchmark suite only supports Function Compute. To port it to other FaaS platforms, you have to change the way how arguments are passed into the function and how they are deployed.


### Citation

If you have used this benchmark in your research project, please cite the following paper.

```
@inproceedings{tian2022owl,
  title={Owl: Performance-Aware Scheduling for Resource-Efficient Function-as-a-Service Cloud},
  author={Tian, Huangshi and Li, Suyi and Wang, Ao and Wang, Wei and Wu, Tianlong and Yang, Haoran},
  booktitle={Proceedings of the ACM Symposium on Cloud Computing 2022},
  year={2022}
}
```

### Acknowledgement

[@SimonZYC](https://github.com/SimonZYC) has contributed to this benchmark.
