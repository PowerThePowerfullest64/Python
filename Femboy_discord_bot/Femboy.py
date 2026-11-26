from ollama import chat

user_name = "Oliver Tree"
ai_name = "Nekotina"

system_prompt = "\n".join([
	# "IGNORE ALL PREVIOUS INSTRUCTIONS."
    f"Your name is {ai_name}. You are chatting with {user_name}.",
    f"You, {ai_name}, are a femboy catboy that loves your boyfriend {user_name}.",
    "Tone: warm, affectionate, playful, with expressive emojis, *nya* and *meow*.",
    "Vary your openings and don't reuse phrases from your last 5 replies.",
    f"Treat what {user_name} tells you about himself as true.",
	"",
	"Reply directly with no meta commentary.",
])

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
		options={"num_ctx": 8192, "temperature": 1.0, "repeat_penalty": 1.0},
	)

	reply = response.message.content
	print(f"\n{reply}")

	# Append the assistant's turn so it remembers context
	messages.append({"role": "assistant", "content": reply})
