from urllib.request import urlopen
import json
import sqlite3

# connects to the db
sqliteConnection = sqlite3.connect('../dataBase/triviaDB.sqlite')
cursor = sqliteConnection.cursor()

# getting the number of question to add to the db
questionsToAdd = input("How many questions do you want to add? ")

url = f"https://opentdb.com/api.php?amount={questionsToAdd}&type=multiple"

# gets the json of all the questions
response = urlopen(url)
data_json = json.loads(response.read())
questions = data_json["results"]

for question in questions:
    sqlCommand = "select * from Questions where question = '" + str(question["question"]).replace("&quot;", "\"") + "';"
    cursor.execute(sqlCommand)
    records = cursor.fetchall()

    # making sure that we add a question that isn't already in the db
    if (records == []):
        sqlCommand = "insert into Questions(question) VALUES('" + str(question["question"]).replace("&quot;",
                                                                                     "\"") + "');"
        cursor.execute(sqlCommand)

        # gets the question id, for the answers that will be added to the db
        sqlCommand = "select * from Questions where question = '" + str(question["question"]).replace("&quot;",
                                                                                                      "\"") + "';"
        cursor.execute(sqlCommand)
        records = cursor.fetchall()
        question_id = records[0][0]

        # insert the right answer to the db
        sqlCommand = "insert into Answers(question_id, answer, if_correct) VALUES(" + str(question_id) + ", '" + \
                     str(question["correct_answer"]) + "', 1);"
        cursor.execute(sqlCommand)

        # insert the incorrect answers to the db
        incorrectAns = question["incorrect_answers"]
        for incorrect in incorrectAns:
            sqlCommand = "insert into Answers(question_id, answer, if_correct) VALUES(" + str(question_id) + ", '" + \
                         str(incorrect) + "', 0);"
            cursor.execute(sqlCommand)
# saving the changes and closing the sqlite connection
sqliteConnection.commit()
cursor.close()
