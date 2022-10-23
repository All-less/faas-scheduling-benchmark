const getRawBody = require('raw-body');
const handle = require('./handler')

/*
if you open the initializer feature, please implement the initializer function, as below:
module.exports.initializer = function(context, callback) {
  console.log('initializing');
  callback(null, '');
};
*/

module.exports.handler = function(req, resp, context) {

    getRawBody(req, function(err, body) {
        const params = {
            path: req.path,
            queries: req.queries,
            headers: req.headers,
            method : req.method,
            requestURI : req.url,
            clientIP : req.clientIP,
            json: JSON.parse(body.toString()),
        }

        handle(params, null, null).then((res) => {
          resp.send(res)
        }).catch((err) => {
          resp.send(JSON.stringify(err))
        })
    })
    
};

