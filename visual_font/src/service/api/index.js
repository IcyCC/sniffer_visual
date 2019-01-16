import axios from 'axios'
axios.defaults.headers.post['Content-Type'] = 'application/json; charset=utf-8';

export const queryRequestInfos = (args) => {
        return axios.post('/api/request_infos', {
            _method: 'GET',
            _args:args
        })
}

export const queryRequestHeaders = (args) => {
    return axios.post('/api/request_headers', {
        _method: 'GET',
        _args:args
    })
}

export const getRquestInfo =(id) => {
    return axios.get('/api/request_infos/' + id)
}