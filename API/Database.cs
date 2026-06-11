using System;
using Microsoft.Data.Sqlite;

class Database
{
    private const string ConnectionString = "Data Source=dev.db";

    public Database()
    {
        InitializeDatabase();
    }

    static void InitializeDatabase()
    {
        using var connection = new SqliteConnection(ConnectionString);
        connection.Open();

        var command = connection.CreateCommand();
        command.CommandText = 
        @"
            CREATE TABLE IF NOT EXISTS keys (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                key TEXT NOT NULL UNIQUE
            );
        ";
        command.ExecuteNonQuery();

        command.CommandText = 
        @"
            CREATE TABLE IF NOT EXISTS data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER NOT NULL,
                title TEXT NOT NULL UNIQUE,
                category TEXT NOT NULL,
                content TEXT,
                FOREIGN KEY (user_id) REFERENCES keys(id)
                ON UPDATE NO ACTION ON DELETE NO ACTION
            );
        ";
        command.ExecuteNonQuery();
    }

    public void InsertIntoKeys(string key)
    {
        using var connection = new SqliteConnection(ConnectionString);
        connection.Open();

        var command = connection.CreateCommand();
        command.CommandText = "INSERT INTO keys (key) VALUES ($key)";
        
        command.Parameters.AddWithValue("$key", key);

        try
        {
            command.ExecuteNonQuery();
            Console.WriteLine($"Successfully inserted key: {key}");
        }
        catch (SqliteException ex) when (ex.SqliteErrorCode == 19)
        {
            Console.WriteLine($"Skipped: User with key '{key}' already exists.");
        }
    }

    public void InsertIntoData(string userKey, List<Database_Entry> entries)
    {
        using var connection = new SqliteConnection(ConnectionString);
        connection.Open();

        using var transaction = connection.BeginTransaction();

        try
        {
            using var command = connection.CreateCommand();
            command.CommandText = @"
                INSERT OR REPLACE INTO data (user_id, title, category, content) 
                VALUES (
                    (SELECT id FROM keys WHERE key = $key),
                    $title,
                    $category,
                    $content
                )
            ";

            foreach (var entry in entries)
            {
                command.Parameters.Clear();
                command.Parameters.AddWithValue("$key", userKey);
                command.Parameters.AddWithValue("$title", entry.Title);
                command.Parameters.AddWithValue("$category", entry.Category);
                command.Parameters.AddWithValue("$content", entry.Content ?? (object)DBNull.Value);
                command.ExecuteNonQuery();
            }

            transaction.Commit();
        }

        catch
        {
            transaction.Rollback();
            throw;
        }
    }
    

    public bool ValidUser(string key)
    {
        using var connection = new SqliteConnection("Data Source=dev.db");
        connection.Open();

        using var command = connection.CreateCommand();
        command.CommandText = "SELECT id from keys where key = ($key)";

        command.Parameters.AddWithValue("$key", key);

        using var reader = command.ExecuteReader();
        
        return reader.Read();
    }

    public List<Database_Entry> PullData(string userKey)
    {
        List<Database_Entry> output = [];

        using var connection = new SqliteConnection("Data Source=dev.db");
        connection.Open();

        using var command = connection.CreateCommand();
        command.CommandText = 
            @"
                SELECT d.id, d.title, d.category, d.content
                FROM data d
                JOIN keys k ON d.user_id = k.id
                WHERE k.key = $key
            ";
        command.Parameters.AddWithValue("$key", userKey);

        using var reader = command.ExecuteReader();
        while (reader.Read())
        {
            output.Add(new Database_Entry(
                reader.GetInt32(0),
                reader.GetString(1),
                reader.GetString(2),
                reader.IsDBNull(3) ? null : reader.GetString(3)
            ));
        }

        return output;
    }
}
