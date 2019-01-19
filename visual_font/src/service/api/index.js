import axios from 'axios'

axios.defaults.headers.post['Content-Type'] = 'application/json; charset=utf-8';

export const queryRequestInfos = (args) => {
    return axios.post('/api/request_infos', {
        _method: 'GET',
        _args: args
    })
}

export const queryRequestHeaders = (args) => {
    return axios.post('/api/request_headers', {
        _method: 'GET',
        _args: args
    })
}

export const getRquestInfo = (id) => {
    return axios.get('/api/request_infos/' + id)
}

export const queryHostClientRank = () => {
    return axios.get('/api/queryHostClientRank')
}

export const queryHostTimetRank = () => {
    return axios.get('/api/queryHostTimeRank')
}

export const queryClient = () => {
    return axios.get('/api/queryClientRank')
}

export const queryClientSrcLike = (src) =>{
    return axios.get('/api/client_src', {params: {'_like_src':src}})
}

export const queryCLientHostRanks = (src) =>{
    return axios.get('/api/queryClientHostRank/'+src)
}

export const queryClientHourTime =(src)=>{
    if (src){
        return axios.get('/api/queryClientHourTime/'+src)
    } else {
        return Promise.reject()
    }
}

export const queryHostSrcLike = (host) =>{
    return axios.get('/api/host_src', {params: {'_like_host':host}})
}

export const queryHostCLientRanks = (host) =>{
    return axios.get('/api/queryHostClientRank/'+host)
}

export const queryHostHourTime =(host)=>{
    if (host){
        return axios.get('/api/queryHostHourTime/'+host)
    } else {
        return Promise.reject()
    }
}