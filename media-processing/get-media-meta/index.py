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
2. service's role has OSSReadAccess
3. event format
{
    "object_url": "http://xxxx.xxx/xxx.mp4"
}
'''

# a decorator for print the excute time of a function
def print_excute_time(func):
    def wrapper(*args, **kwargs):
        local_time = time.time()
        ret = func(*args, **kwargs)
        LOGGER.info('current Function [%s] excute time is %.2f seconds' %
                    (func.__name__, time.time() - local_time))
        return ret
    return wrapper


@print_excute_time
def handler(env, start_resp):
    size = int(env.get('CONTENT_LENGTH', 0))
    evt = json.loads(env['wsgi.input'].read(size))
    object_url = evt["object_url"]

    cmd = ["/code/ffprobe", "-v", "quiet", "-show_format", "-show_streams",
           "-print_format", "json",  "-i",  object_url]
    try:
        raw_result = subprocess.check_output(cmd)
        start_resp('200 OK', [('Content-type', 'text/plain')])
        return [ raw_result ]
    except subprocess.CalledProcessError as e:
        start_resp('200 OK', [('Content-type', 'text/plain')])
        return [ json.dumps({
            'code': e.returncode,
            'cmd': e.cmd,
            'stdout': e.stdout.decode('utf-8') if e.stdout is not None else '',
            'stderr': e.stderr.decode('utf-8') if e.stderr is not None else ''
        }).encode('utf-8') ]
