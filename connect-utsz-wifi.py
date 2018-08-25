#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import requests

r = requests.post('10.0.10.66/srun_portal_pc.php?ac_id=1&url=http://220.181.57.216/', data={'action': 'login', 'ac_id': '1', 'user_ip'})
