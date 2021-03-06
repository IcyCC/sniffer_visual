from sqlalchemy import create_engine, MetaData, Table


def get_engine(user, password, host, port, database):
    print('mysql+pymysql://{user}:{password}@{host}:{port}/{database}?charset=utf8mb4'.format(
            user=user,
            password=password,
            host=host,  # your host
            port=port,
            database=database,
        ))
    engine = create_engine(
        'mysql+pymysql://{user}:{password}@{host}:{port}/{database}?charset=utf8mb4'.format(
            user=user,
            password=password,
            host=host,  # your host
            port=port,
            database=database,
        )
    )
    return engine


class Tables(object):
    """
    用于操作 mysql 的table对象
    """

    def __init__(self, user, password, host, port, database):
        self.user = user
        self.password = password
        self.host = host
        self.port = port
        self.database = database
        self._engine = None
        self._metadata = None
        self._tables = None

    def connect(self):
        self._engine = get_engine(user=self.user, password=self.password, host=self.host, port=self.port,
                                  database=self.database)
        self._metadata = MetaData(self._engine)
        self._metadata.reflect(bind=self._engine)
        self._tables = self._metadata.tables

    def __getitem__(self, name):
        return self._tables[name]

    def __getattr__(self, item):
        return self._tables[item]

    def execute(self, sql, *args, **kwargs):
        conn = self._engine.connect()
        return conn.execute(sql, *args,**kwargs)
