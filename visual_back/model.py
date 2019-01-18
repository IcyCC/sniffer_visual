from db_util import Tables as MyTable

my_table = MyTable('root', 'Root!!2019', 'localhost', 3306, 'web')

my_table.connect()
