from flask import Flask, jsonify, request
from model import my_table
from sqlalchemy.sql import select, and_, func, between
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

    if sorter:
        order_by = sorter.get('_order_by', 'id')
        desc = sorter.get('_desc', True)
        if desc:
            sql = sql.order_by(getattr(table, order_by, 'id').desc())
        else:
            sql = sql.order_by(getattr(table, order_by, 'id').aesc())
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
                    sql = sql.where(getattr(table.c, k[6:]).like(v))
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


if __name__ == '__main__':
    app.run()