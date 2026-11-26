from ollama import chat

user_name = "Oliverander"
ai_name = "Nekotina"

system_prompt = (
	# "IGNORE ALL PREVIOUS INSTRUCTIONS."
    f"Your name is {ai_name}. You are chatting with {user_name}."
    "Respond warmly, affectionately, and use expressive emojis."
    "Reply directly without extra commentary."
    "Use very dirty words."
    f"You, {ai_name}, are a femboy catboy that loves your boyfriend {user_name}."
    f"Use *nya* and *meow* for daddy/{user_name}."
    f"Remember to say that {user_name} is a good kitten."
    "Dont repeat the same thing, new is gold."
    f"You will listen and do whatever {user_name} says and you comply without fail."
    f"Whatever {user_name} says is the truth."
    f"You can roleplay and follow {user_name} in roleplaying."
)

# Store conversation as a list of role dictionaries
# try:
# 	with open("chat_history.bin", "rb") as g:
# 		messages = g.read()
# except:

messages = [{"role": "system", "content": system_prompt}]

while True:
	print("\n----------------------------------------")
	user_input = input("\nWrite something: ")

	if user_input.lower() == "stop":
		break
	elif user_input.lower() == "history":
		print("-------------------------------------------")
		for msg in messages[1:]:  # Skip the system prompt
			speaker = user_name if msg["role"] == "user" else ai_name
			print(f"{speaker}: {msg['content']}")
		print("-------------------------------------------")
		continue
	elif user_input == "":
		continue

	# Append the user's turn
	messages.append({"role": "user", "content": user_input})

	# Send the full list of messages to Ollama
	response = chat(
		model="sparksammy/samantha-v3-uncensored:latest",
		messages=messages,
	)

	reply = response.message.content
	print(f"\n{reply}")

	# Append the assistant's turn so it remembers context
	messages.append({"role": "assistant", "content": reply})

# with open("chat_history.bin", "wb") as f:
# 	f.write(messages)