extern crate kafka;
extern crate custom_error;
extern crate regex;

use std::str;
use std::env;
use std::time::Duration;

use custom_error::custom_error;
use kafka::producer::{Producer, Record, RequiredAcks};
use kafka::consumer::{Consumer, FetchOffset, GroupOffsetStorage};
use kafka::error::Error as KafkaError;
use regex::Regex;


custom_error! { pub ProjError
  Decoding{ source: str::Utf8Error } = "error decoding utf-8",
  Kafka{ source: KafkaError } = "error interacting with Kafka",
}

pub fn handle(_req : String) -> String {
    return match consume() {
        Ok(count) => format!("Successfully filtered {} messages.", count),
        Err(e) => format!("Error occurs. {}", e),
    };
}

pub fn consume() -> Result<usize, ProjError> {

    // to_owned() convert a string literal into a String.
    let broker = match env::var("KAFKA_BROKER") {
        Ok(val) => val,
        Err(_) => "kafka-headless.kafka.svc.cluster.local:9092".to_owned(),
    };
    let read_topic = match env::var("KAFKA_READ_TOPIC") {
        Ok(val) => val,
        Err(_) => "original".to_owned(),
    };
    let write_topic = match env::var("KAFKA_WRITE_TOPIC") {
        Ok(val) => val,
        Err(_) => "filtered".to_owned(),
    };
    let group = "filter".to_owned();

    let mut con = try!(Consumer::from_hosts(vec![broker.clone()])
        .with_topic(read_topic)
        .with_group(group)
        .with_fallback_offset(FetchOffset::Earliest)
        .with_offset_storage(GroupOffsetStorage::Kafka)
        .create());
    println!("after creating con");
    let mut producer = try!(Producer::from_hosts(vec!(broker.clone()))
        .with_ack_timeout(Duration::from_secs(1))
        .with_required_acks(RequiredAcks::One)
        .create());

    let mut msg: Vec<String> = Vec::new();
    let re = Regex::new(r".*input_userauth_request.*").unwrap();

    loop {
        let mss = try!(con.poll());

        if mss.is_empty() { return Ok(msg.len()); }

        for ms in mss.iter() {
            for m in ms.messages() {
                // println!("{}:{}@{}: {:?}", ms.topic(), ms.partition(), m.offset, m.value);
                if !re.is_match(str::from_utf8(m.value).unwrap()) {
                    producer.send(&Record::from_value(write_topic.as_str(), m.value)).unwrap();
                }
                msg.push(try!(str::from_utf8(m.value)).to_string());
            }
            let _ = con.consume_messageset(ms);
        }
        // try!(con.commit_consumed());
    }
}

