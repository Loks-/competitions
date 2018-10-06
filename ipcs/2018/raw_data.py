import requests

session = requests.Session()
URL_LOGIN = "https://ipsc.ksp.sk/2018/login"
login_request = session.post(url = URL_LOGIN, data={"username" : "Loks", "password" : "q1w2e3"})
URL_R1 = "https://ipsc.ksp.sk/2018/practice/problems/r1"
r = session.post(url = URL_R1) 
r = session.post(url = URL_R1) 
r = session.post(url = URL_R1) 
URL_R2 = "https://ipsc.ksp.sk/2018/practice/problems/r2"
r = session.post(url = URL_R2) 
r = session.post(url = URL_R2) 
r = session.post(url = URL_R2) 
