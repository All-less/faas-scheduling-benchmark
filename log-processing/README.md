
## Log Processing Application

This application mimics a storage service provider that processes its logs according to regulation requirement.
- The [Anonymize Log](#anonymize-log-function) function anonymizes the sensitive personal information in the log.
- The [Filter Log](#filter-log-function) function filters the logs and stores them to an object storage.

### Prerequisites

Both functions require a running [Kafka](https://kafka.apache.org/).
- After deploying Kafka, edit the `KAFKA_BROKER` and `KAFKA_TOPIC` entries in `{anonymize-log,filter-log}/func/template.yml`.
    - Create three topics in Kafka: `original-logs`, `anonymized-logs` and `filtered-logs`.
    - Then publish the logs in `data/sample-logs.logs` to `original-logs`.

### Anonymize Log Function

To build the function,

```
cd faas-scheduling-benchmark/log-processing/anonymize-log
make deploy-anonymization
```

To invoke the function,

```
# The <fc-endpoint> can be found in the result printed out by `fun deploy`.
curl -s -X POST -i http://<fc-endpoint>/2016-08-15/proxy/log-processing/anonymize-log/
```

### Filter Log Function

To build the function,

```
cd faas-scheduling-benchmark/log-processing/filter-log
make deploy-filter
```

To invoke the function,

```
# The <fc-endpoint> can be found in the result printed out by `fun deploy`.
curl -s -X POST -i http://<fc-endpoint>/2016-08-15/proxy/log-processing/filter-log/
```
