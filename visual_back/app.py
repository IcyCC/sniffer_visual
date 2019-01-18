from flask import Flask, jsonify, request
import pandas as pd
from model import my_table
from sqlalchemy.sql import select, and_, func, between, distinct, text
from util import convert_datetime_to_string

app = Flask(__name__)


def comm_query(table_name, query, pager, sorter):
    table = my_table[table_name]
    sql = select([table])
    if query:
        for k, values in query.items():
            if not values:
                continue
            if k.startswith('_gt_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[4:]) > v)
            elif k.startswith('_gte_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[5:]) >= v)
            elif k.startswith('_lt_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[4:]) < v)
            elif k.startswith('_lte_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[5:]) <= v)
            elif k.startswith('_like_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[6:]).like(v))
            else:
                sql = sql.where(getattr(table.c, k).in_(values))

    if pager:
        per_page = pager.get('_per_page')
        page = pager.get('_page')
        if per_page:
            sql = sql.limit(per_page)
        if page:
            if per_page is None:
                sql = sql.offset((page - 1) * 30).limit(30)
            else:
                sql = sql.offset((page - 1) * per_page)
    if sorter is None:
        sorter = {}
    order_by = sorter.get('_order_by', 'id')
    desc = sorter.get('_desc', True)
    if desc:
        sql = sql.order_by(getattr(table.c, order_by, table.c.id).desc())
    else:
        sql = sql.order_by(getattr(table.c, order_by, table.c.id))
    res = my_table.execute(sql)
    return res.fetchall()


def comm_count(table_name, query):
    table = my_table[table_name]
    sql = select([func.count('*')], from_obj=table)
    if query:
        for k, values in query.items():
            if not values:
                continue
            if k.startswith('_gt_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[4:]) > v)
            elif k.startswith('_gte_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[5:]) >= v)
            elif k.startswith('_lt_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[4:]) < v)
            elif k.startswith('_lte_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[5:]) <= v)
            elif k.startswith('_like_'):
                for v in values:
                    sql = sql.where(getattr(table.c, k[6:]).like("%" + v))
            else:
                sql = sql.where(getattr(table.c, k).in_(values))

    res = my_table.execute(sql)
    return res.scalar()


def url_condition(args):
    query = {}
    pager = {}
    sorter = {}
    if args:
        for k, v in args.items():
            if k == '_per_page':
                pager['_per_page'] = v
            elif k == '_page':
                pager['_page'] = v
            elif k == '_order_by':
                sorter['_order_by'] = v
            elif k == '_desc':
                sorter['order'] = v
            else:
                if isinstance(v, list):
                    query[k] = v
                else:
                    query[k] = [v]
    return query, pager, sorter


def format_request_info(data):
    res = dict(data)
    res['created_at'] = convert_datetime_to_string(data['created_at'])
    return res


def format_request_header(data):
    res = dict(data)
    return res


@app.route("/request_infos", methods=["POST"])
def query_request_infos():
    body = request.json
    if body.get('_method', '') == 'GET':
        query, pager, sorter = url_condition(body.get("_args"))
        res = comm_query('request_infos', query, pager, sorter)
        count = comm_count('request_infos', query)
        return jsonify(code=200, msg='', request_infos=[format_request_info(i) for i in res], total=count)


@app.route("/request_infos/<int:id>", methods=["GET"])
def get_request_infos(id):
    res = comm_query('request_infos', {'id': [id]}, {'_per_page': 1}, None)
    if res:
        return jsonify(code=200, msg='', request_info=format_request_info(res[0]))
    else:
        return jsonify(code=404, msg='', request_info={})


@app.route("/request_headers", methods=["POST"])
def query_request_headers():
    body = request.json
    if body.get('_method') == 'GET':
        query, pager, sorter = url_condition(body.get("_args"))
        res = comm_query('request_headers', query, pager, sorter)
        count = comm_count('request_headers', query)
        return jsonify(code=200, msg='', request_headers=[format_request_header(i) for i in res], total=count)


@app.route("/request_headers/<int:id>", methods=["GET"])
def get_request_header(id):
    res = comm_query('request_headers', {'id': [id]}, {'_per_page': 1}, None)
    if res:
        return jsonify(code=200, msg='', request_header=format_request_info(res[0]))
    else:
        return jsonify(code=404, msg='', request_header={})


@app.route("/queryHostClientRank", methods=["GET"])
def query_host_client_rank():
    res = my_table.execute("""
SELECT request_infos.host as host, COUNT(DISTINCT src) as num FROM request_infos GROUP BY host ORDER BY COUNT(DISTINCT src) DESC; 
""")
    res = res.fetchall()
    return jsonify(code=200, msg='', host_ranks=[dict(i) for i in res])


@app.route("/queryHostTimeRank", methods=["GET"])
def query_host_time_rank():
    res = my_table.execute("""
    SELECT request_infos.host as host, COUNT(*) as num FROM request_infos GROUP BY host ORDER BY  COUNT(*) DESC;
    """)
    res = res.fetchall()
    return jsonify(code=200, msg='', host_ranks=[dict(i) for i in res])


@app.route("/queryClientRank", methods=["GET"])
def query_client_rank():
    res = my_table.execute("""
    SELECT  request_infos.src, count(*) as num FROM request_infos GROUP BY request_infos.src ORDER BY COUNT(*) DESC;
    """)
    res = res.fetchall()
    return jsonify(code=200, msg='', client_ranks=[dict(i) for i in res])


@app.route("/client_src", methods=["GET"])
def query_client_src():
    src_like = request.args.get('_like_src', '')
    sql = select([distinct(my_table.request_infos.c.src)]).where(my_table.request_infos.c.src.like("%" + src_like))
    sql = sql.limit(30)
    res = my_table.execute(sql).fetchall()
    return jsonify(code=200, msg='', client_srcs=[dict(i) for i in res])


@app.route("/queryClientHostRank/<string:src>", methods=["GET"])
def query_clint_host_ranks(src):
    res = my_table.execute(text("""
    SELECT request_infos.host as host, COUNT(*) as num FROM request_infos WHERE src = :src GROUP BY host;
    """), src=src)
    res = res.fetchall()
    return jsonify(code=200, msg='', client_hosts=[dict(i) for i in res])


@app.route("/queryClientHourTime/<string:src>", methods=["GET"])
def query_client_hour_time(src):
    request_infos = comm_query("request_infos", {'src': [src]}, None, {'_order_by': 'id', '_desc': False})
    request_infos = {convert_datetime_to_string(i.created_at): 1 for i in request_infos}
    df = pd.Series(request_infos)
    df.index = pd.to_datetime(df.index)
    res = df.resample('15min')
    return jsonify(code=200, msg='', client_time=[dict(time=convert_datetime_to_string(k), num=v) for k,v in res.sum().to_dict().items()])


if __name__ == '__main__':
    app.run(host='0.0.0.0')
