#pragma once

#include <string>

std::string Send(const std::string& s);

/*
API_KEY = '3bd205ec3d2640ac9b73eccecf9d540e'

def send(data, debug=True):
    if debug:
        print(f'Sending {data}')
    if type(data) == list and len(data) == 3 and type(data[2]) == list and
len(data[2]) == 1 and type(data[2][0]) == list and len(data[2][0]) == 3: last =
data[2][0][2] if type(last) == list and len(last) == 2: data[2][0][2] =
tuple(data[2][0][2]) if debug: print(f'Actually sending {data}') data =
to_str(modulate_anything(data)) response =
requests.post(f'https://icfpc2020-api.testkontur.ru/aliens/send?apiKey={API_KEY}',
data=data).text res = demodulate(response) if debug: print(f'Res: {res}') return
res

def send2(data, server_url='https://icfpc2020-api.testkontur.ru'):
    print(f'Sending {data}')
    response = requests.post(f'{server_url}/aliens/send?apiKey={API_KEY}',
data=to_str(modulate_anything(data))).text res = demodulate(response) assert
res[0] == 1, (data, res) return res

if __name__ == "__main__":
    #assert send(0) == [0]
    send([4, 7364416995030719050, [[0, 0, (1, 1)]]])
*/
