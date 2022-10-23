# -*- coding: utf-8 -*-
import subprocess
import oss2
import logging
import json
import os
import time

logging.getLogger("oss2.api").setLevel(logging.ERROR)
logging.getLogger("oss2.auth").setLevel(logging.ERROR)

LOGGER = logging.getLogger()

'''
1. function and bucket locate in same region
2. service's role has OSSFullAccess
3. event format
{
    "object_url": "http://xxxx.xxx/xxx.mp3",
    "output_bucket": "test",
    "output_prefix": "",
    "dst_type": "wav",
    "params": "-ac 1 -ar 4000"

    "bucket_name" : "test-bucket",
    "object_key" : "a.mp3",
    "output_dir" : "output/",
    "dst_type": ".wav",
    "ac": 1,
    "ar": 4000
}
'''

def handler(env, start_resp):
    context = env['fc.context']
    size = int(env.get('CONTENT_LENGTH', 0))
    body = json.loads(env['wsgi.input'].read(size))

    object_url = body['object_url']
    name = object_url.split('/')[-1].split('.')[0]
    dst_type = body['dst_type']
    path = f'/tmp/{name}.{dst_type}'

    cmd = ['/code/ffmpeg', '-i', object_url] + \
           body['params'].split(' ') + [path]
    try:
        subprocess.run(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    except subprocess.CalledProcessError as e:
        start_resp('200 OK', [('Content-type', 'text/plain')])
        return [ json.dumps({
            'code': e.returncode,
            'cmd': e.cmd,
            'stdout': e.stdout.decode('utf-8') if e.stdout is not None else '',
            'stderr': e.stderr.decode('utf-8') if e.stderr is not None else ''
        }).encode('utf-8') ]

    creds = context.credentials
    auth = oss2.StsAuth(creds.accessKeyId,
                        creds.accessKeySecret, creds.securityToken)
    oss_client = oss2.Bucket(auth,
        'oss-%s-internal.aliyuncs.com' % context.region, body['output_bucket'])

    creds = context.credentials
    auth = oss2.StsAuth(creds.accessKeyId,
                        creds.accessKeySecret, creds.securityToken)
    oss_client = oss2.Bucket(
        auth, 'oss-%s-internal.aliyuncs.com' % context.region, body['output_bucket'])

    key = os.path.join(body['output_prefix'], f'{name}.{dst_type}')
    oss_client.put_object_from_file(key, path)
    os.remove(path)

    start_resp('200 OK', [('Content-type', 'text/plain')])
    return [ 'ok'.encode('utf-8') ]
