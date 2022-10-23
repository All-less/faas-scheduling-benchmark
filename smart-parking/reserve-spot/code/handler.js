'use strict'

const { promisify } = require('util')

module.exports = async (params, context, callback) => {

  try {
    const redis = require('redis')
    const url = process.env.REDIS_URL
    const client = redis.createClient({ url })
    const asyncDecr = promisify(client.decr).bind(client)
    const vacancy = await asyncDecr('vacancy')
    client.quit()  // remember to close client
  } catch (e) {
    return 'Error operating Redis: ' + e
  }

  try {
    const { Kafka } = require('kafkajs')
    const producer = new Kafka({
      clientId: 'reserve-spot-client',
      brokers: [ process.env.KAFKA_BROKER ],
      ssl: false,
      sasl: null,
    }).producer()
    await producer.connect()

    const message = JSON.stringify({
      user: params.json['user-id'],
      time: Date.now(),
      operation: {
        name: 'RESERVE',
        site: params.json['site-id'],
        spot: params.json['spot-id'],
      }
    })
    await producer.send({
      topic: process.env.KAFKA_TOPIC,
      messages: [ { value: message  } ],
    })
  } catch (e) {
    return 'Error operating Kafka: ' + e
  }

  return 'Reservation succeeded.'
}
