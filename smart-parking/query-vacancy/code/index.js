const getRawBody = require('raw-body');
const handle = require('./handler')

module.exports.handler = function(req, resp, context) {

    const params = {
        path: req.path,
        queries: req.queries,
        headers: req.headers,
        method : req.method,
        requestURI : req.url,
        clientIP : req.clientIP,
    }

    getRawBody(req, function(err, body) {
        handle(null, null).then((res) => {
          resp.send(res)
        }).catch((err) => {
          resp.send(JSON.stringify(err))
        })
    })

};

