from datetime import datetime, timedelta

def convert_datetime_to_string(datetime_time, format="%Y-%m-%d %H:%M:%S"):
    return datetime_time.strftime(format) if datetime_time else ''
