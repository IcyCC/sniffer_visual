from db_util import Tables as MyTable

my_table = MyTable('root', 'qds161002103', 'localhost', 3306, 'web')

my_table.connect()
