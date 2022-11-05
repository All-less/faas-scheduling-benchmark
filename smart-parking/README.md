
## Smart Parking Application

This application manages smart parking sites and provides a mobile app to users.
- The [Query Vacancy](#query-vacancy-function) function queries the vacancy of a specific spot.
- The [Reserve Spot](#reserve-spot-function) function reserves a specific parking spot.

### Prerequisites

Both functions require a running [Redis](https://redis.io/) and the Reserve Spot function further needs [Kafka](https://kafka.apache.org/).
- After deploying Redis, edit the `REDIS_URL` entry in both `{query-vacancy,reserve-spot}/code/template.yml`. Then create a key-value pair `vacancy:10` in Redis.
- After deploying Kafka, create a topic with the name of your choice. Then edit the `KAFKA_BROKER` and `KAFKA_TOPIC` entries in `reserve-spot/code/template.yml`.

### Query Vacancy Function

To deploy the function,

```
cd faas-scheduling-benchmark/smart-parking/query-vacancy/code
npm i  # install node packages

cd ..
fun deploy -y
```

To invoke the function,

```
# The <fc-endpoint> can be found in the result printed out by `fun deploy`.
curl -s -i http://<fc-endpoint>/2016-08-15/proxy/smart-parking/query-vacancy/
```

### Reserve Spot Function

To deploy the function,

```
cd faas-scheduling-benchmark/smart-parking/reserve-spot/code
npm i

cd ..
fun deploy -y
```

To invoke the function,

```
curl -s -i http://<fc-endpoint>/2016-08-15/proxy/smart-parking/reserve-spot/ \
        -X POST \
        -d '{"spot-id": "1234", "user-id": "5678", "site-id": "1234" }' \
        -H 'Content-Type: application/json'
```
