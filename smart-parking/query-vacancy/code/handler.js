'use strict'

const { promisify } = require('util')

module.exports = async (context, callback) => {
  const redis = require('redis')
  const url = process.env.REDIS_URL
  try {
    const client = redis.createClient({ url })
    const getAsync = promisify(client.get).bind(client)
    const res = await getAsync('vacancy')
    client.quit()  // remember to close client
    return 'Current vacancy is ' + (res || 0) + '\n'
  } catch (e) {
    return 'Error querying vacancy: ' + e + '\n'
  }
}
