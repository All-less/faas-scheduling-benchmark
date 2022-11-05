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
        Ok(count) => format!("Successfully anonymized {} messages.", count),
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
        Err(_) => "anonymized".to_owned(),
    };
    let group = "anonymize".to_owned();

    let mut con = try!(Consumer::from_hosts(vec![broker.clone()])
        .with_topic(read_topic)
        .with_group(group)
        .with_fallback_offset(FetchOffset::Earliest)
        .with_offset_storage(GroupOffsetStorage::Kafka)
        .create());
    let mut producer = try!(Producer::from_hosts(vec!(broker.clone()))
        .with_ack_timeout(Duration::from_secs(1))
        .with_required_acks(RequiredAcks::One)
        .create());

    let mut msg: Vec<String> = Vec::new();
    let re = Regex::new(r"(?P<h>\d{1,3}.\d{1,3}.\d{1,3}.)(?P<t>\d{1,3})").unwrap();

    loop {
        let mss = try!(con.poll());

        if mss.is_empty() { return Ok(msg.len()); }

        for ms in mss.iter() {
            for m in ms.messages() {
                // println!("{}:{}@{}: {:?}", ms.topic(), ms.partition(), m.offset, m.value);
                let replaced = re.replace_all(str::from_utf8(m.value).unwrap(), "$h***");
                producer.send(&Record::from_value(write_topic.as_str(), replaced.as_bytes())).unwrap();
                msg.push(try!(str::from_utf8(m.value)).to_string());
            }
            let _ = con.consume_messageset(ms);
        }
        // try!(con.commit_consumed());
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_regex() {
        use super::*;
        let re = Regex::new(r"(?P<h>\d{1,3}.\d{1,3}.\d{1,3}.)(?P<t>\d{1,3})").unwrap();
        let array: &[u8] = &[84, 104, 105, 115, 32, 105, 115, 32, 97, 32, 108, 111, 103,
                             32, 108, 105, 110, 101, 32, 102, 114, 111, 109, 32, 49, 50,
                             51, 46, 52, 53, 54, 46, 55, 56, 57, 46, 49, 50, 51, 46];
        let replaced = re.replace_all(str::from_utf8(array).unwrap(), "$h***");
        assert_eq!("This is a log line from 123.456.789.***.", replaced);
    }
}
